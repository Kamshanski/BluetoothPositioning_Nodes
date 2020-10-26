#pragma once

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include <HardwareSerial.h>

#include <utilities.h>
#include <MACAddress.h>
#include <TargetsSet.h>

/**
 * Main class to operatre with BLE connections and device collection
 */
class Net : public BLEServerCallbacks{
private:
    BLEServer *server;                  //
    BLEAdvertising *adv;                // 
    BLEAdvertisementData *advData;      // Data to be advertized (no use for positioning)
    int devicesNum = 0;                 // Number of connected devices
    bool connected = false;             // Flag (no use. If many  devices connects simultaniously then change {deviceAddress} to array)
    MACAddress *deviceAddress;          // Temp var to hold adr if device connected. Must be released in onDisconnect       
    TargetsSet *targetsSet;
    ~Net() {}
    
public:
    Net();
	
    void sendNewDeviceToTheMinors();

	// BLE Server Callbacks
    void onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param);
    void onDisconnect(BLEServer* pServer);
  
};
static const std::string MANUF_DATA = "__devices:";
