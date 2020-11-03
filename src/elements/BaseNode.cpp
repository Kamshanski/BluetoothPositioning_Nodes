#include <Node.h>

BaseNode::BaseNode(std::string nodeName, uint8_t deviceId) {    
    BLEDevice::init(nodeName);
    BLEDevice::setPower(ESP_PWR_LVL_P9);
    this->deviceId = deviceId;                  
}

void BaseNode::initScanTool() {
    scanner = BLEDevice::getScan();
    // Incert true to have duplicates (for RSSI tracking purpose)
    scanner->setAdvertisedDeviceCallbacks(this, true);
    scanner->setActiveScan(false);
    scanner->setWindow(10);
    scanner->setInterval(2);
}

// Set duration to 0 for eternal scan
void BaseNode::scan(int duration) {
    if (!scanning) {
        // 0 allows to use eternal scan. Scanning can be stopped only by ->stop() method.
        scanning = true;
        scanner->start(duration);
    }
}

void BaseNode::scanStop() {
    if (scanning) {
        scanning = false;
        scanner->stop();
    }
}

void BaseNode::onResult(BLEAdvertisedDevice advertisedDevice) {
    uint8_t msg[ESP_BD_ADDR_LEN + 2];
    esp_bd_addr_t* addr = advertisedDevice.getAddress().getNative();
    copyAddress(*addr, msg);

    int devicePos = targetsSet->find(msg);
    if (devicePos >= 0) {
        uint8_t rssi = ((uint8_t) -advertisedDevice.getRSSI());       // To insert rssi in 8 bits (one char)

        msg[ESP_BD_ADDR_LEN ] = rssi;
        msg[ESP_BD_ADDR_LEN + 1] = deviceId;
        
        processNewMsg(msg);         // Slaves send rssi to Main. Main sends data to server
        
    }
}

void BaseNode::onScanStoped(BLEScan* scanner) {
    prl("scan stop");
}


