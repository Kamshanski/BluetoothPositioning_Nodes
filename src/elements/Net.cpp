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

    BLEDevice::startAdvertising();
}

void Net::sendNewDeviceToTheMinors() {

}

void Net::onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) {
    connected = true;

    pServer->disconnect(param->connect.conn_id);
    MACAddress ad = MACAddress(param->connect.remote_bda);      // There must be a reason to create separate var to pass it in the set insead of direct passing
    targetsSet->addOrRemove(ad);

    sendNewDeviceToTheMinors();

    Serial.println("Device connected");         
}

void Net::onDisconnect(BLEServer* pServer) {
    connected = false;
    Serial.println("Device DISconnected");
}

