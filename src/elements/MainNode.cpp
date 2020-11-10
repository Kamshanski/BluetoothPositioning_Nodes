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
        pr("Slave Connected: ");
        prl(slavesConnected);
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
    Serial.println(out.c_str());
    Serial.println("Device connected");       
}

//WARNING! This method was added to esp32 BLE library by ME, to handle MAC of disconnected device!
void MainNode::onDisconnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) {
    // Save received data
    uint8_t * receivedAddr = new uint8_t[ESP_BD_ADDR_LEN];
    copyAddress(param->connect.remote_bda, receivedAddr);
    
    if (isAnyOfSlaves(param->disconnect.remote_bda)) {
        slavesConnected--;
        pr("Slave DISconnected: ");
        prl(slavesConnected);
        return;
    }
    
    Serial.println("Device DISconnected");
}

void MainNode::onWrite(BLECharacteristic* pCharacteristic) {
    std::string payload = pCharacteristic->getValue();
    char receivedMsg[MSG_LEN];

    for (int i = 0; i < MSG_LEN; i++) {
        receivedMsg[i] = payload[i];
    }
    
    pushToServer(receivedMsg);
    
    // // Print
    // char receivedAddr[MAC_LEN_IN_MSG];
    // for (int i = 0; i < MAC_LEN_IN_MSG; i++) {
    //     receivedAddr[i] = receivedMsg[i];
    // }

    // char rssi[RSSI_LEN_IN_MSG];
    // int r, r1, r2, rs = 0;
    // for (int i = 0; i < RSSI_LEN_IN_MSG; i++) {
    //     rssi[i] = receivedMsg[MAC_LEN_IN_MSG + i];
    // }

    // if ((receivedMsg[MAC_LEN_IN_MSG+1] >= '0') && (receivedMsg[MAC_LEN_IN_MSG+1] <= '9')) {
    //     r1 = (receivedMsg[MAC_LEN_IN_MSG+1] - '0');
    // } else {
    //     r1 = (receivedMsg[MAC_LEN_IN_MSG+1] - 'a');
    // }

    // if ((receivedMsg[MAC_LEN_IN_MSG+2] >= '0') && (receivedMsg[MAC_LEN_IN_MSG+2] <= '9')) {
    //     r2 = receivedMsg[MAC_LEN_IN_MSG+2] - '0';
    // } else {
    //     r2 = receivedMsg[MAC_LEN_IN_MSG+2] - 'a';
    // }
    // r = (r1 << 4) + r2;
    // rs = receivedMsg[MAC_LEN_IN_MSG] == '-' ? -r : r;

    // char devId[ID_LEN_IN_MSG];
    // for (int i = 0; i < ID_LEN_IN_MSG; i++) {
    //     rssi[i] = receivedMsg[MAC_LEN_IN_MSG + RSSI_LEN_IN_MSG + i];
    // }

    // int receivedDeviceId = (int) receivedMsg[POS_DEVICE_ID];
    // Serial.print("ON_WRITE: Addr: ");
    // Serial.write((const uint8_t *)receivedAddr, MAC_LEN_IN_MSG);
    // // print
    // Serial.print(", RSSI: ");
    // Serial.print(rs);
    // Serial.print(", By device: ");
    // Serial.print(receivedDeviceId);
    //pushToServer(receivedAddr, rssi, receivedDeviceId);
}

void MainNode::processNewMsg(char* msg) {
    prl();
    pushToServer(msg);
}

void MainNode::pushToServer(char* msg) {
    int bufferState = serverBuffer->put(msg);
    if (bufferState <= 0) {
        if (serverBuffer->getSize() > 49)
            serverBuffer->send();    
    }   
}


void MainNode::onConnect(BLEServer* pServer) {}
//WARNING! This method was added to esp32 BLE library by ME, to handle MAC of disconnected device!
void MainNode::onDisconnect(BLEServer* pServer) {}
