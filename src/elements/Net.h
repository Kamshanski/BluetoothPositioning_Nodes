#pragma once

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include <BLE2902.h>
#include <HardwareSerial.h>
#include <MACAddresses.h>
#include <utilities.h>
#include <TargetsSet.h>

#define MAIN_SERVICE_UUID "c33d30da-0cad-40e6-94f6-097c504e6ee2"
#define NEW_DEVICES_CHARACTERISTIC_UUID "25338aa2-79be-4164-aaa3-f35011f93d7b"

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
    BLEService *service;                // Service 
    BLECharacteristic *notificationCharacteristic;
    int slaveIndex = 0;
    TargetsSet *targetsSet;
    ~Net() {}
    
    void copyAddress(uint8_t * from, uint8_t * to);
    bool isAnyOfSlaves(uint8_t * addr);
	
    void publishUpdate(uint8_t *addr, const char * option);
    std::string* createMessageForSlaves(uint8_t *addr, const char * option);
    void conntectToAllSlaves();

	// BLE Server Callbacks
    void onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param);
    void onDisconnect(BLEServer* pServer);
public:
    Net();
};
static const std::string MANUF_DATA = "__devices:";
static const char * ADD = "N";
static const char * REMOVE = "D";
