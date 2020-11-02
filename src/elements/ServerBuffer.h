#include <stdint.h>
#include <string>


// ESP_BD_ADDR_LEN
#define RSSI_MSG_LEN 8
#define POS_RSSI ESP_BD_ADDR_LEN
#define POS_DEVICE_ID ESP_BD_ADDR_LEN+1

class ServerBuffer {
private:
    int maxMsgs;
    int bufferLen;
    std::string buffer;
    int size = 0;
public:
    ServerBuffer(int maxMsgs) {
        this->maxMsgs = maxMsgs;
        this->bufferLen = RSSI_MSG_LEN * maxMsgs;
        buffer.reserve(bufferLen);
    }
    int put(uint8_t *msg);    
    void send();    
};