#include <esp_bt_defs.h>
#include <stdint.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <string.h>
#include <stdio.h>


// ESP_BD_ADDR_LEN
#define RSSI_MSG_LEN 8
#define POS_RSSI ESP_BD_ADDR_LEN
#define POS_DEVICE_ID ESP_BD_ADDR_LEN+1

class ServerBuffer {
private:
    int maxMsgs;
    std::stringstream stream;
    int size = 0;
    unsigned long timeStart = 0UL;
    const char HEX_LETTERS[17] = "0123456789abcdef";
    static const uint8_t FIRST = 0b00001111;
    static const uint8_t SECOND = 0b11110000;

    void clear();
public:
    ServerBuffer(int maxMsgs) {
        this->maxMsgs = maxMsgs;
    }
    int put(uint8_t *msg);    
    void send();    
};