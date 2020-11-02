#pragma once

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLEClient.h>
#include <BLEScan.h>
#include <BLE2902.h>

#include "Esp.h"

#include <HardwareSerial.h>
#include <MACAddresses.h>
#include <ServerBuffer.h>
#include <utilities.h>
#include <AddressSet.h>

#define MAIN_SERVICE_UUID "c33d30da-0cad-40e6-94f6-097c504e6ee2"
#define NEW_DEVICES_CHARACTERISTIC_UUID "25338aa2-79be-4164-aaa3-f35011f93d7b"
#define RSSI_COLLECT_CHARACTERISTIC_UUID "b05ad2c8-d560-4560-a868-7022fc06ec70"
#define SYNC_CHARACTERISTIC_UUID "bc358e4b-972b-448a-a83c-ae784d68a037"
#define RSSI_TRANSMISSION_BIAS 100

class BaseNode : public BLEAdvertisedDeviceCallbacks {
protected:
    uint8_t deviceId;
    BLEScan* scanner;
    AddressSet *targetsSet;
    int devicesNum = 0;                 // Number of connected devices
    bool scanning = false;

public:
    BaseNode(std::string nodeName, uint8_t deviceId);
    ~BaseNode() {}
    
    void initScanTool();
    void scan(int duration);
    void scanStop();
    bool isScanning() { return scanning; }

    void onResult(BLEAdvertisedDevice advertisedDevice);
	void onScanStoped(BLEScan* scanner);
    virtual void processNewMsg(uint8_t* msg) = 0;
};

/**
 * Main class to operatre with BLE connections and device collection
 */
class MainNode : public BaseNode, public BLEServerCallbacks, public BLECharacteristicCallbacks{
private:
    BLEService *service;                            // Service 
    BLECharacteristic *notificationCharacteristic;  // MainNode publishes new devices through this characteristic
    BLECharacteristic *syncCharacteristic;          // Slaves use it to obtain all devises that are already connected to MainNode
    BLECharacteristic *rssiCollectCharacteristing;  // Slaves put rssi they discovered inside this characteristic
    BLEServer *server;                              //
    BLEAdvertising *adv;                            // 
    BLEAdvertisementData *advData;                  // Data to be advertized (no use for positioning)

    ServerBuffer *serverBuffer;

    int slavesConnected = 0;                        //
    
    void publishUpdate(uint8_t *addr, const char * option);
    std::string* createMessageForSlaves(uint8_t *addr, const char * option);
    void setSyncData();
    void processNewMsg(uint8_t* msg);
    void pushToServer(uint8_t* msg);
	// BLE Server Callbacks
	void onWrite(BLECharacteristic* pCharacteristic);
    void onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param);
    void onDisconnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param);
public:
    ~MainNode() {}
    MainNode(std::string nodeName, uint8_t deviceId);
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
    BLERemoteCharacteristic* syncCharacteristic = nullptr;
    BLERemoteCharacteristic* rssiCollectCharacteristing = nullptr;
    bool hasRssiToSend = false;
    uint8_t sendingBuffer[ESP_BD_ADDR_LEN+2] = {0};
    
    void processNewMsg(uint8_t* msg);
    
public:
    SlaveNode(std::string nodeName, uint8_t deviceId);
    ~SlaveNode() {}
    
    bool connectToMainNode();
    void syncTargetsSet();

    bool hasRssiDiscovered();
    void sendToMain();

    // BLE Client and Characteristics Callbacks
    void onNotify(BLERemoteCharacteristic *remoteCharacteristic, esp_ble_gattc_cb_param_t *evtParam);
    void onConnect(BLEClient* pClient, esp_ble_gatts_cb_param_t *param);
    void onDisconnect(BLEClient *pClient, esp_ble_gatts_cb_param_t *param);
};

// Used to fill registerForNotify() method of BLERemoteCharacteristic. Otherwise no notifications will appear
static void nullFunction(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) { }


