#include <ServerBuffer.h>
#include <Arduino.h>
#include <utilities.h>
/** Returns -1 if buff is alredy full
 *          0 if buff became full after put()
 *          1 if buff isn't full
 */
int ServerBuffer::put(char *msg) {
    if (timeStart == 0UL) {
        timeStart = millis();
    }

    if (size < maxMsgs) {
        int pos = (size << 4) + size;    // size * MSG_LEN, as MSG_LEN = 8 = 2^3
        for (int i = 0; i < MSG_LEN; i++) {
            buffer[pos + i] = msg[i];
            // buffer[pos + 1];
            // int rssi = msg[7];
            // addNum(msg[])
            // char first = HEX_LETTERS[msg[i] & FIRST];
            // char second = HEX_LETTERS[(msg[i] & SECOND) >> 4];
            // stream << std::setfill('0') << std::setw(2) << std::hex << (int) msg[i];
        }
        size++;
        return size < maxMsgs;
    } else {
        return -1;
    }
}

void ServerBuffer::send() {
    if (!isSending) {
        isSending = true;
        pr("TS:"); prl(timeStart);
        pr("TE:"); prl(millis());
        pr("Payload:"); prl(buffer.c_str());
        clear();
        isSending = false;
        return;
    }
}

void ServerBuffer::clear() {
    size = 0;
    timeStart = 0UL;  
}
