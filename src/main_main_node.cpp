#include "Arduino.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <utilities.h>
#include <StringWrap.h>

class ServerListener : public BLEServerCallbacks {

    void onConnect(BLEServer* pServer) {
        Serial.println("Device connect");
    }
    void onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) {
        Serial.println("Device connected");
        delay(10);
        // connected = true;
        StringWrap* s = new StringWrap();
        Serial.println("1");
        char buffer[2];
        Serial.println("2-----");
        for (int i = 0; i < ESP_BD_ADDR_LEN; i++) {
            Serial.println("3");
            itoa(param->connect.remote_bda[i], buffer, 10);
            Serial.println("4");
            s->append(buffer);
            Serial.println("5");
        }
        Serial.println("6-----");
        Serial.println(s->copy());
        Serial.println("7");
        delete s;
        /* deviceAddress = copyAddress(param->connect.remote_bda); */
        pServer->disconnect(param->connect.conn_id);
        Serial.println("Device connected");         
	}
    void onDisconnect(BLEServer* pServer) {
        // connected = false;
        // bool isInNet = checkIfConnected(deviceAddress);
        // if (isInNet) {
        //     addTarget(deviceAddress);
        //     notifyMinorsDeviceAdded(deviceAddress);
        // } 
        // else {
        //     removeTarget(deviceAddress);
        //     notifyMinorsDeviceRemoved(deviceAddres);
        // }
        Serial.println("Device DISconnected");
	}
};

class Net{
private:
    BLEServer *server;
    BLEAdvertising *adv;
    BLEAdvertisementData *advData;
    int devicesNum = 0;
    bool connected = false;
    esp_bd_addr_t deviceAddress;        // Temp var to hold adr if device connected. Must be released in onDisconnect
    ~Net() {}

	// esp_bd_addr_t copyAddress(esp_bd_addr_t orig) {
    //     esp_bd_addr_t addr = esp_bd_addr_t();
    //     for (size_t i = 0; i < ESP_BD_ADDR_LEN; i++) {
    //         addr[i] = orig[i];
    //     }
    // }
    
public:
    static const std::string MANUF_DATA; 
    Net() {
        BLEDevice::init("Main_node");

        server = BLEDevice::createServer();
        server->setCallbacks(new ServerListener());

        adv = BLEDevice::getAdvertising();
        adv->setScanResponse(true);
        adv->setMinPreferred(0x06); // functions that help with iPhone connections issue
        adv->setMinPreferred(0x12);

        advData = new BLEAdvertisementData();
        advData->setManufacturerData(MANUF_DATA);
        MANUF_DATA.substr(0, MANUF_DATA.length()).append(deviceNumToString());
        adv->setAdvertisementData(*advData);

        BLEDevice::startAdvertising();
    }

    std::string* getAdvData() {
        char buffer[MANUF_DATA.length()];
        MANUF_DATA.copy(buffer, MANUF_DATA.length(), 0);
    }

    char* deviceNumToString() {
        char buffer[2];
        return itoa(devicesNum, buffer, 10);
    }

    

    void onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) {
			Serial.println("Device connected");
            delay(10);
            connected = true;
            std::string s = std::string();
            char buffer[2];
            for (int i = 0; i < ESP_BD_ADDR_LEN; i++) {
                itoa(param->connect.remote_bda[i], buffer, 10);
                s.append(buffer);
            }
            Serial.println(s.c_str());
            /* deviceAddress = copyAddress(param->connect.remote_bda); */
            pServer->disconnect(param->connect.conn_id);
            Serial.println("Device connected");         
	}

    void onDisconnect(BLEServer* pServer) {
        connected = false;
        // bool isInNet = checkIfConnected(deviceAddress);
        // if (isInNet) {
        //     addTarget(deviceAddress);
        //     notifyMinorsDeviceAdded(deviceAddress);
        // } 
        // else {
        //     removeTarget(deviceAddress);
        //     notifyMinorsDeviceRemoved(deviceAddres);
        // }
        prl("Device DISconnected");
	}

    
};
const std::string Net::MANUF_DATA = "__devices:";

// GLOBALS
Net* net;

void setup() {
    Serial.begin(115200);
    net = new Net();
    Serial.println("Device started");
}

void loop() {
}
