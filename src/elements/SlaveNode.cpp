#include <Node.h>

SlaveNode::SlaveNode(std::string nodeName) : BaseNode(nodeName) {
    client = BLEDevice::createClient();         //
    client->setClientCallbacks(this);           // this class implements onConnect and onDisconnect
    
    connectToMainNode();                        // Connect to MainNode instantly
    prl("Connection to MainNode successfully established");
     
    targetsSet = new AddressSet(10);            // Collection of devices 
    
    scan(0); 
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
    char buff[MAC_ADDRESS_STRING_LENGTH];
    addrToString(receivedAddr, buff);
    pr(buff);
    if (option == ADD[0]) {
        targetsSet->add(receivedAddr);
        prl(" added");
    }
    else if (option == REMOVE[0]) {
        targetsSet->remove(receivedAddr);
        prl(" removed");
    } else if (option == NO_DATA[0]) {
        prl(" - no data");
    }
    std::string report = targetsSet->toString();    
    prl(report.c_str());
}

bool SlaveNode::connectToMainNode() {
    while (!client->connect(*MAIN_ADDR)) {
        prl("Connecting to MainNode");
    } 
    prl("Connected..."); 

    serviceOfMainNode = client->getService(MAIN_SERVICE_UUID);
    if (serviceOfMainNode == nullptr) {
        Serial.print("Failed to find our service UUID: ");  //  "Не удалось найти UUID нашего сервиса: "
        return false;
    }
    prl("Service is found...");

    notificationCharacteristic = serviceOfMainNode->getCharacteristic(NEW_DEVICES_CHARACTERISTIC_UUID);

    if (notificationCharacteristic == nullptr || notificationCharacteristic == nullptr) {
        Serial.print("Failed to find our characteristic UUID");
        return false;
    }
    prl("Characteristic is found...");

    notificationCharacteristic->setCallback(this);
    notificationCharacteristic->registerForNotify(nullFunction);
    prl("Callback is set...");
    
    return true;
}
