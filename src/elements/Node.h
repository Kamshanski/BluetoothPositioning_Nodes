#pragma once

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLEClient.h>
#include <BLEScan.h>

#include <BLE2902.h>
#include <HardwareSerial.h>
#include <MACAddresses.h>
#include <utilities.h>
#include <AddressSet.h>

#define MAIN_SERVICE_UUID "c33d30da-0cad-40e6-94f6-097c504e6ee2"
#define NEW_DEVICES_CHARACTERISTIC_UUID "25338aa2-79be-4164-aaa3-f35011f93d7b"
#define SCANNING_PERIOD_SEC 3

class BaseNode : public BLEAdvertisedDeviceCallbacks {
protected:
    BLEScan* scanner;
    AddressSet *targetsSet;
    int devicesNum = 0;                 // Number of connected devices
    bool scanning = false;

public:
    BaseNode(std::string nodeName);
    ~BaseNode() {}
    
    void scan(int duration);
    bool isScanning() { return scanning; }

    void onResult(BLEAdvertisedDevice advertisedDevice);
	void onScanStoped(BLEScan* scanner);
};

/**
 * Main class to operatre with BLE connections and device collection
 */
class MainNode : public BaseNode, public BLEServerCallbacks{
private:
    BLEService *service;                // Service 
    BLECharacteristic *notificationCharacteristic;
    BLEServer *server;                  //
    BLEAdvertising *adv;                // 
    BLEAdvertisementData *advData;      // Data to be advertized (no use for positioning)
    
    int slavesConnected = 0;            //
    
    void publishUpdate(uint8_t *addr, const char * option);
    std::string* createMessageForSlaves(uint8_t *addr, const char * option);

	// BLE Server Callbacks
    void onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param);
    void onDisconnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param);
public:
    ~MainNode() {}
    MainNode(std::string nodeName);
};
static const std::string MANUF_DATA = "__devices:";
static const char * const ADD = "N";
static const char * const REMOVE = "D";
static const char * const NO_DATA = "0";

class SlaveNode : public BaseNode, public BLEClientCallbacks, public BLERemoteCharacteristicCallback{
private:
    BLEClient *client;
    BLERemoteService* serviceOfMainNode = nullptr;
    BLERemoteCharacteristic* notificationCharacteristic = nullptr;
    
    bool connectToMainNode();
    
public:
    SlaveNode(std::string nodeName);
    ~SlaveNode() {}

    // BLE Client and Characteristics Callbacks
    void onNotify(BLERemoteCharacteristic *remoteCharacteristic, esp_ble_gattc_cb_param_t *evtParam);
    void onConnect(BLEClient* pClient, esp_ble_gatts_cb_param_t *param);
    void onDisconnect(BLEClient *pClient, esp_ble_gatts_cb_param_t *param);
};

// Used to fill registerForNotify() method of BLERemoteCharacteristic. Otherwise no notifications will appear
static void nullFunction(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) { }


