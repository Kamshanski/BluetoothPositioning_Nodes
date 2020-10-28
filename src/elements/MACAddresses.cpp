#include <MACAddresses.h>
#include <HardwareSerial.h>

bool isAnyOfSlaves(uint8_t * addr) {
    for (int i = 0; i < SLAVES_NUMBER; i++) {
        bool equals = true;
        esp_bd_addr_t *slave = SLAVES_ADDR[i]->getNative();
        for (int j = 0; (j < ESP_BD_ADDR_LEN) & (equals == true); j++) {
            equals = (*slave)[j] == addr[j];
        }
        if (equals == true) {
            return true;
        }
    }
    return false;
}

void copyAddress(uint8_t * from, uint8_t * to) {
    for (int i = 0; i < ESP_BD_ADDR_LEN; i++) {
        to[i] = from[i];
    }
}