#include <ServerBuffer.h>
#include <Arduino.h>
/** Returns -1 if buff is alredy full
 *          0 if buff became full after put()
 *          1 if buff isn't full
 */
int ServerBuffer::put(uint8_t *msg) {
    if (timeStart == 0UL) {
        timeStart = millis();
        stream  << "{TS:" << timeStart << ",payload:[";
    }

    if (size < maxMsgs) {
        for (int i = 0; i < RSSI_MSG_LEN; i++) {
            char first = HEX_LETTERS[msg[i] & FIRST];
            char second = HEX_LETTERS[(msg[i] & SECOND) >> 4];
            stream << std::setfill('0') << std::setw(2) << std::hex << (int) msg[i];
        }
        size++;
        
        return size < maxMsgs;
    }
    else {
        return -1;
    }
}

void ServerBuffer::send() {
    stream << "],timeE:" << millis() << ",}";
    Serial.println(stream.str().c_str());
    clear();
}

void ServerBuffer::clear() {
    size = 0;
    timeStart = 0UL;
    stream.clear();    
}
