#include <Node.h>

SlaveNode::SlaveNode(std::string nodeName, uint8_t deviceId) : BaseNode(nodeName, deviceId) {
    client = BLEDevice::createClient();         //
    client->setClientCallbacks(this);           // this class implements onConnect and onDisconnect

    prl("Connection to MainNode successfully established");
    targetsSet = new AddressSet(10);            // Collection of devices 
    
}

void SlaveNode::onConnect(BLEClient* pClient, esp_ble_gatts_cb_param_t *param) {
    prl("Somejne Connected!");
}
void SlaveNode::onDisconnect(BLEClient* pClient, esp_ble_gatts_cb_param_t *param) {
    prl("Somejne DISconnected!");
}

void SlaveNode::onNotify(BLERemoteCharacteristic *remoteCharacteristic, esp_ble_gattc_cb_param_t *evtParam) {
    uint8_t receivedAddr[ESP_BD_ADDR_LEN];

    int dataLen = evtParam->notify.value_len;
    char receivedData[dataLen];
    for (int i = 0; i < dataLen; i++) {
        receivedData[i] = (char) evtParam->notify.value[i];
    }
    pr("Received Data: ");
    prl(receivedData);
    copyAddress(&(evtParam->notify.value[1]), receivedAddr);
    char option = evtParam->notify.value[0];
    if (option == ADD[0]) {
        targetsSet->add(receivedAddr);
    }
    else if (option == REMOVE[0]) {
        targetsSet->remove(receivedAddr);
    } else if (option == NO_DATA[0]) {
        prl(" - no data");
    }
    
    char buff[MAC_ADDRESS_STRING_LENGTH];
    addrToString(receivedAddr, buff);
    pr(buff);
}

bool SlaveNode::connectToMainNode() {
    if (!client->connect(*MAIN_ADDR)) {
        prl("Restarting slave because no connection was established");
        abort();
    } 
    prl("Connected..."); 

    serviceOfMainNode = client->getService(MAIN_SERVICE_UUID);
    if (serviceOfMainNode == nullptr) {
        Serial.print("Failed to find our service UUID: ");  //  "Не удалось найти UUID нашего сервиса: "
        return false;
    }
    prl("Service is found...");

    notificationCharacteristic = serviceOfMainNode->getCharacteristic(NEW_DEVICES_CHARACTERISTIC_UUID);
    if (notificationCharacteristic == nullptr) {
        Serial.print("Failed to find notificationCharacteristic");
        return false;
    }
    prl("NotificationCharacteristic is found...");
    notificationCharacteristic->setCallback(this);
    notificationCharacteristic->registerForNotify(nullFunction);
    prl("NotificationCharacteristic Callback is set...");

    rssiCollectCharacteristing = serviceOfMainNode->getCharacteristic(RSSI_COLLECT_CHARACTERISTIC_UUID);
    if (rssiCollectCharacteristing == nullptr) {
        Serial.print("Failed to find rssiCollectCharacteristing");
        return false;
    }
    prl("RssiCollectCharacteristing is found...");
    
    return true;
}

void SlaveNode::syncTargetsSet() {
    syncCharacteristic = serviceOfMainNode->getCharacteristic(SYNC_CHARACTERISTIC_UUID);
    if (syncCharacteristic == nullptr) {
        Serial.print("Failed to find Sync characteristic UUID");
        return ;
    }
    prl("SyncCharacteristic is found...");
    std::string syncData = syncCharacteristic->readValue();
    Serial.println(syncData.c_str());
    
    /* Тут надо добаить парсинг сихро записи, 
    но сначала поменять отправляемые сообщения на более компактные */

    prl("Callback is set...");
    
}

void SlaveNode::processNewMsg(uint8_t* msg) {
    for (int i = 0; i < RSSI_MSG_LEN; i++) {
        sendingBuffer[i] = msg[i];
    }          
    
    hasRssiToSend = true;
}

bool SlaveNode::hasRssiDiscovered() {
    return hasRssiToSend;
}
void SlaveNode::sendToMain() {
    rssiCollectCharacteristing->writeValue(sendingBuffer, ESP_BD_ADDR_LEN+2, false);
    hasRssiToSend = false;
    prl("sent");
}
