#include <Net.h>

Net::Net() {
    BLEDevice::init("Main_node");               // 

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

    targetsSet = new TargetsSet(10);

    service = server->createService(MAIN_SERVICE_UUID);
    notificationCharacteristic = service->createCharacteristic(
        NEW_DEVICES_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ   |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_INDICATE
    );
    notificationCharacteristic->addDescriptor(new BLE2902());
    notificationCharacteristic->setValue(targetsSet->getAddress(targetsSet->getSize()));
    notificationCharacteristic->notify();
    service->start();

    BLEDevice::startAdvertising();
}

void Net::publishUpdate(uint8_t *addr, const char * option) {
    std::string* msg = createMessageForSlaves(addr, option);
    notificationCharacteristic->setValue(*msg);
    notificationCharacteristic->notify();
    conntectToAllSlaves();
}

std::string* Net::createMessageForSlaves(uint8_t *addr, const char * option) {
    std::string *payload = new std::string(option);
    char * buff = new char[TargetsSet::MAC_ADDRESS_STRING_LENGTH];
    TargetsSet::addrToString(addr, buff);
    payload->append(")")
            .append(buff)
            .append(")");

    return payload;
}

void Net::conntectToAllSlaves() {
    for (int i = 0; i < SLAVES_NUMBER; i++) {
        if (!server->connect(*(SLAVES_ADDR[i]))) {
            Serial.print("\nCoonection FAILED with _");
            Serial.print(i);
            Serial.println("_th slave!!!");
        }
    }
}

void Net::onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) {
    connected = true;

    uint8_t * receivedAddr = new uint8_t[ESP_BD_ADDR_LEN];
    uint16_t receivedConnId = param->connect.conn_id;
    copyAddress(param->connect.remote_bda, receivedAddr);

    // No reaction if slave is connected
    if (isAnyOfSlaves(receivedAddr))
        return;
    
    pServer->disconnect(receivedConnId);

    int devicePos = 0;
    devicePos = targetsSet->find(receivedAddr);
    if (devicePos < 0) {
        // Device is new. It should be recorded to targetsSet and published to Slaves
        targetsSet->add(receivedAddr);
        publishUpdate(receivedAddr, ADD);
    } 
    else {
        // Device is new. It should be excluded from targetsSet and its removal must be published to Slaves
        targetsSet->remove(devicePos);
        publishUpdate(receivedAddr, REMOVE);
    }

    Serial.println("Device connected");         
}

void Net::onDisconnect(BLEServer* pServer) {
    connected = false;
    Serial.println("Device DISconnected");
}

bool Net::isAnyOfSlaves(uint8_t * addr) {
    bool equals = true;
    for (int i = 0; i < SLAVES_NUMBER; i++) {
        esp_bd_addr_t *slave = SLAVES_ADDR[i]->getNative();
        for (int j = 0; (j < ESP_BD_ADDR_LEN) & (equals == true); j++) {
            equals = (*slave)[j] == addr[j];
        }
        if (equals == true) {
            return true;
        }
    }
    return false;
}

void Net::copyAddress(uint8_t * from, uint8_t * to) {
    for (int i = 0; i < ESP_BD_ADDR_LEN; i++) {
        to[i] = from[i];
    }
}
