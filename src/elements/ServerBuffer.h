#include <esp_bt_defs.h>
#include <stdint.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <string.h>
#include <stdio.h>


// ESP_BD_ADDR_LEN
#define MSG_LEN 17
#define MAC_LEN_IN_MSG 12
#define RSSI_LEN_IN_MSG 3
#define ID_LEN_IN_MSG 2
#define POS_RSSI ESP_BD_ADDR_LEN
#define POS_DEVICE_ID ESP_BD_ADDR_LEN+1

class ServerBuffer {
private:
    int maxMsgs;
    std::string buffer;
    int size = 0;
    int isSending = false;
    unsigned long timeStart = 0UL;
    const char HEX_LETTERS[17] = "0123456789abcdef";
    static const uint8_t FIRST = 0b00001111;
    static const uint8_t SECOND = 0b11110000;

    void clear();
    void initBuffer() {
        buffer.reserve(850);
    } 


template<typename T>
void addHexNum(T x, int startPos) {
    T mask = 0xFUL << (sizeof(T) - 1); 
    int shift = (sizeof(T) - 1) << 2;
    for (int i = 0; i < sizeof(T); i++) {
        int index = (x >> shift) & 0xFUL;
        buffer[startPos + i] = HEX_LETTERS[index];
        mask = mask >> 4;
        shift = shift - 4;
    }
}

void addText(char * text, int length, int startPos) {
    for (int i = 0; i < length; i++) {
        buffer[startPos + i] = text[i];
    }
}

public:
    ServerBuffer(int maxMsgs) {
        this->maxMsgs = maxMsgs;
        initBuffer();
    }
    int put(char *msg);    
    void send();  
    int getSize() {return size;}
};