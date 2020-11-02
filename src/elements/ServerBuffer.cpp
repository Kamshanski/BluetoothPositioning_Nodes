#include <ServerBuffer.h>
#include <Arduino.h>
/** Returns -1 if buff is alredy full
 *          0 if buff became full after put()
 *          1 if buff isn't full
 */

int ServerBuffer::put(uint8_t *msg) {
    if (size < maxMsgs) {
        int pos = size * RSSI_MSG_LEN;
        for (int i = 0; i < RSSI_MSG_LEN; i++) {
            buffer[pos] = msg[i];
            pos++;
        }
        size++;
        
        return size < maxMsgs;
    }
    else {
        return -1;
    }
}

void ServerBuffer::send() {
    std::string metadata("T");

    int sizeOfTimeBuff = 8 * sizeof(unsigned long);
    char buf[sizeOfTimeBuff + 1];
    ultoa(millis(), buf, 10);
    
    metadata.append(buf, sizeOfTimeBuff)
            .append(".");
    
    Serial.print("MSG_STARTS");
    Serial.print(metadata.c_str());
    Serial.print(buffer.c_str());
    Serial.print("END_OF_MSG");
    size = 0;
}