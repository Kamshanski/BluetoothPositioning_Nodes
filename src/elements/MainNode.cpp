#include <Node.h>

MainNode::MainNode(std::string nodeName) : BaseNode(nodeName) {
    server = BLEDevice::createServer();         //
    server->setCallbacks(this);                 // this class implements onConnect and onDisconnect
    adv = BLEDevice::getAdvertising();          // Get object to set advirtizing
    adv->setScanResponse(true);                 // 
    adv->setMinPreferred(0x06);                 // functions that help with iPhone connections issue
    adv->setMinPreferred(0x12);
    advData = new BLEAdvertisementData();       // New class must be created each time ('cause lib-creators are cunts)
    advData->setManufacturerData(MANUF_DATA);   // Manufacturer Data is where additional data can be advertized
    //MANUF_DATA.substr(0, MANUF_DATA.length())   
    //          .append(deviceNumToString());     // херня, переписать. сабстринг надо пихать в отдельную переменную и потом сувать в advData
    adv->setAdvertisementData(*advData);        //
    targetsSet = new AddressSet(10); 
    service = server->createService(MAIN_SERVICE_UUID);
    notificationCharacteristic = service->createCharacteristic(
        NEW_DEVICES_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ   |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_INDICATE
    );
    notificationCharacteristic->addDescriptor(new BLE2902());
    std::string str = targetsSet->getAddress(targetsSet->getSize());
    prl(str.c_str());
    notificationCharacteristic->setValue(str);
    //notificationCharacteristic->notify();     // Можно ставить только, если есть какие-либо подключённые клиенты.
    service->start();
    BLEDevice::startAdvertising();
}

void MainNode::publishUpdate(uint8_t *addr, const char * option) {
    std::string* msg = createMessageForSlaves(addr, option);
    notificationCharacteristic->setValue(*msg);
    if (slavesConnected > 0) {
        // Causes crash if no slaves connected to main
        notificationCharacteristic->notify();
    }
}

std::string* MainNode::createMessageForSlaves(uint8_t *addr, const char * option) {
    std::string *payload = new std::string(option);

    char * buff = new char[AddressSet::MAC_ADDRESS_STRING_LENGTH];
    AddressSet::addrToString(addr, buff);

    payload->append(")")
            .append(buff)
            .append(")");

    return payload;
}

void MainNode::onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) {
    // Save received data
    Serial.print("Device tries to connect: ");  
    uint8_t * receivedAddr = new uint8_t[ESP_BD_ADDR_LEN];
    uint16_t receivedConnId = param->connect.conn_id;
    copyAddress(param->connect.remote_bda, receivedAddr);

    BLEDevice::startAdvertising();  
    
    // No reaction if slave is connected. Just count it's number
    if (isAnyOfSlaves(receivedAddr)) {
        slavesConnected++;
        prl("Slave Connected!");
        return;
    }
    pServer->disconnect(receivedConnId);

    int devicePos = targetsSet->find(receivedAddr);
    if (devicePos < 0) {
        // Device is new. It should be recorded to targetsSet and published to Slaves
        targetsSet->add(receivedAddr);
        publishUpdate(receivedAddr, ADD);
    } 
    else {
        // Device is already connected. It should be excluded from targetsSet and its removal must be published to Slaves
        targetsSet->remove(devicePos);
        publishUpdate(receivedAddr, REMOVE);
    }
    Serial.print("Slaves connected: ");
    Serial.println(slavesConnected);
    std::string out = targetsSet->toString();
    Serial.print(out.c_str());
    Serial.println("Device connected");       
}

//WARNING! This method was added to esp32 BLE library by ME, to handle MAC of disconnected device!
void MainNode::onDisconnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) {
    // Save received data
    uint8_t * receivedAddr = new uint8_t[ESP_BD_ADDR_LEN];
    copyAddress(param->connect.remote_bda, receivedAddr);
    
    if (isAnyOfSlaves(param->disconnect.remote_bda)) {
        slavesConnected--;
        prl("Slave DISconnected!");
        return;
    }

    Serial.print("Slaves connected: ");
    Serial.println(slavesConnected);
    
    Serial.println("Device DISconnected");
}



