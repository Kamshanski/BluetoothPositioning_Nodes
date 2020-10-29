#include <Node.h>

BaseNode::BaseNode(std::string nodeName) {    
    BLEDevice::init(nodeName);                  
    scanner = BLEDevice::getScan();
    // Use true to have duplicates (for RSSI tracking purposes)
    scanner->setAdvertisedDeviceCallbacks(this, true);
    scanner->setActiveScan(false);
    scanner->setWindow(ADV_TYPE_SCAN_IND);
    scanner->setInterval(ADV_TYPE_SCAN_IND);
}

// Set duration to 0 for eternal scan
void BaseNode::scan(int duration) {
    if (!isScanning()) {
        // 0 allows to use eternal scan. Scanning can be stopped only by ->stop() method.
        scanner->start(duration);
    }
}

void BaseNode::onResult(BLEAdvertisedDevice advertisedDevice) {
    esp_bd_addr_t* addr = advertisedDevice.getAddress().getNative();
    int devicePos = targetsSet->find((uint8_t*)addr);
    if (devicePos >= 0) {
        int rssi = advertisedDevice.getRSSI();
        Serial.print("Device found: "); //  "Устройство найдено. Подключаемся!"
        Serial.print(advertisedDevice.getName().c_str());
        Serial.print(" : ");
        Serial.println(rssi);
    }
    
}
void BaseNode::onScanStoped(BLEScan* scanner) {

}

