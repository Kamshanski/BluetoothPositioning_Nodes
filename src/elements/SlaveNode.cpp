#include <Node.h>

SlaveNode::SlaveNode(std::string nodeName) : BaseNode(nodeName) {
    client = BLEDevice::createClient();         //
    client->setClientCallbacks(this);           // this class implements onConnect and onDisconnect
    
    connectToMainNode();                        // Connect to MainNode instantly
    prl("Connection to MainNode successfully established");
     
    targetsSet = new AddressSet(10);            // Collection of devices  
}

void SlaveNode::onConnect(BLEClient* pClient, esp_ble_gatts_cb_param_t *param) {
    prl("Somejne Connected!");
}

void SlaveNode::onDisconnect(BLEClient* pClient, esp_ble_gatts_cb_param_t *param) {
    prl("Somejne DISconnected!");
}

void SlaveNode::onNotify(BLERemoteCharacteristic *remoteCharacteristic, 
                         esp_ble_gattc_cb_param_t *evtParam) {
    Serial.println((char *)evtParam->notify.value);
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
