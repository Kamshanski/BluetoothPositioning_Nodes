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
    char msg[12+3+2];    // Hex of MAC (12), sign of RSSI (1), HEX of RSSI (2), HEX of deviceId
    esp_bd_addr_t* addr = advertisedDevice.getAddress().getNative();
       

    int devicePos = targetsSet->find(*addr);
    if (devicePos >= 0) {
        // MAC address
        
        for (int i = 0; i < ESP_BD_ADDR_LEN; i++) {
            uint8_t ad = (*addr)[i];
            int msgIndex = i << 1;

            uint8_t i1 = (ad >> 4) & 0b00001111;
            uint8_t i2 = (ad) & 0b00001111;

            msg[msgIndex]       = HEX_NUM[i1];
            msg[msgIndex + 1]   = HEX_NUM[i2];
        }
        
        // RSSI with sign
        int rssi = advertisedDevice.getRSSI();
        msg[12] = (rssi < 0) ? '-' : '0';       // Set minus sign or zero
        rssi = rssi < 0 ? -rssi : rssi;         // Get absolute val of rssi
        msg[13] = HEX_NUM[(rssi >> 4) & 0b00001111];
        msg[14] = HEX_NUM[rssi & 0b00001111];

        // Device ID
        msg[15] = HEX_NUM[(deviceId >> 4) & 0b00001111];
        msg[16] = HEX_NUM[deviceId & 0b00001111];
        //Serial.println(msg);
        
        processNewMsg(msg);         // Slaves send rssi to Main. Main sends data to server
    }
}

void BaseNode::onScanStoped(BLEScan* scanner) {
    prl("scan stop");
}


