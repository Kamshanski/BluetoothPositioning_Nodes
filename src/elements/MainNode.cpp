#include <Node.h>

MainNode::MainNode(std::string nodeName, uint8_t deviceId) : BaseNode(nodeName, deviceId) {
    server = BLEDevice::createServer();         //
    server->setCallbacks(this);                 // MainNode class implements onConnect and onDisconnect
    adv = BLEDevice::getAdvertising();          // Get object to set advirtizing
    adv->setScanResponse(true);                 // 
    adv->setMinPreferred(0x06);                 // functions that help with iPhone connections issue
    adv->setMinPreferred(0x12);
    advData = new BLEAdvertisementData();       // New class must be created each time ('cause lib-creators are cunts)
    advData->setManufacturerData(MANUF_DATA);   // Manufacturer Data is where additional data can be advertized
    adv->setAdvertisementData(*advData);        //

    targetsSet = new AddressSet(10);
    serverBuffer = new ServerBuffer(50); 

    service = server->createService(MAIN_SERVICE_UUID);

    notificationCharacteristic = service->createCharacteristic(
        NEW_DEVICES_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ   |
        BLECharacteristic::PROPERTY_INDICATE |
        BLECharacteristic::PROPERTY_NOTIFY
    );
    notificationCharacteristic->addDescriptor(new BLE2902());
    notificationCharacteristic->setValue("0");

    syncCharacteristic = service->createCharacteristic(
        SYNC_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ
    );
    syncCharacteristic->addDescriptor(new BLE2902());
    syncCharacteristic->setValue("0");

    rssiCollectCharacteristing = service->createCharacteristic(
        RSSI_COLLECT_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE    |
        BLECharacteristic::PROPERTY_INDICATE |
        BLECharacteristic::PROPERTY_NOTIFY

    );
    rssiCollectCharacteristing->addDescriptor(new BLE2902());
    rssiCollectCharacteristing->setCallbacks(this);             // MainNode class implements onConnect and onDisconnect

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
    setSyncData();
    delete msg;
}

std::string* MainNode::createMessageForSlaves(uint8_t *addr, const char * option) {
    std::string *payload = new std::string(option);

    char * buff = new char[ESP_BD_ADDR_LEN];
    for (int i = 0; i < ESP_BD_ADDR_LEN; i++) {
        buff[i] = (char) addr[i];
    }

    payload->append(buff);

    return payload;
}

void MainNode::setSyncData() {
    /* Тут надо поменать это сообщение на более компактное */
    syncCharacteristic->setValue(targetsSet->toString());
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
    pServer->disconnect(receivedConnId);     // No need to keep target devices connected


    int devicePos = targetsSet->find(receivedAddr);
    if (devicePos < 0) {
        // Device is new. It should be recorded to targetsSet and published to Slaves
        targetsSet->add(receivedAddr);
        publishUpdate(receivedAddr, ADD);
        prl("Device ADD. Published to slaves...");
    } 
    else {
        // Device is already connected. It should be excluded from targetsSet and its removal must be published to Slaves
        targetsSet->remove(devicePos);
        publishUpdate(receivedAddr, REMOVE);
        prl("Device REMOVE. Published to slaves...");
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


void MainNode::onWrite(BLECharacteristic* pCharacteristic) {
    pr("ON_WRITE: ");

    std::string payload = pCharacteristic->getValue();
    uint8_t receivedMsg[RSSI_MSG_LEN];

    for (int i = 0; i < RSSI_MSG_LEN; i++) {
        receivedMsg[i] = payload[i];
    }
    
    pushToServer(receivedMsg);

    uint8_t receivedAddr[ESP_BD_ADDR_LEN]; 
    copyAddress(receivedMsg, receivedAddr);

    int rssi = ((int) receivedMsg[POS_RSSI]) - RSSI_TRANSMISSION_BIAS;

    int receivedDeviceId = (int) receivedMsg[POS_DEVICE_ID];
    Serial.print("Addr: ");
    for (int i = 0; i < ESP_BD_ADDR_LEN; i++) {
        pr(receivedAddr[i]);
    }
    // print
    Serial.print(", RSSI: ");
    Serial.print(rssi);
    Serial.print(", By device: ");
    Serial.print(receivedDeviceId);
    Serial.print("Memory: ");
    Serial.print(ESP.getFreeHeap());
    Serial.print("SRAM: ");
    Serial.println(ESP.getFreePsram());
    //pushToServer(receivedAddr, rssi, receivedDeviceId);
}

void MainNode::processNewMsg(uint8_t* msg) {
    pushToServer(msg);
}

void MainNode::pushToServer(uint8_t* msg) {
    int bufferState = serverBuffer->put(msg);
    if (bufferState <= 0) {
        serverBuffer->send();    
    }
}




