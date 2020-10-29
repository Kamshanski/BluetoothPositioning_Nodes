#include <MACAddresses.h>
#include <HardwareSerial.h>
#include <utilities.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <string.h>
#include <stdio.h>


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

void addrToString(uint8_t * addr, char * buff) {
    std::stringstream stream;
    stream << std::setfill('0') << std::setw(2) << std::hex << (int) addr[0] << ':';
    stream << std::setfill('0') << std::setw(2) << std::hex << (int) addr[1] << ':';
    stream << std::setfill('0') << std::setw(2) << std::hex << (int) addr[2] << ':';
    stream << std::setfill('0') << std::setw(2) << std::hex << (int) addr[3] << ':';
    stream << std::setfill('0') << std::setw(2) << std::hex << (int) addr[4] << ':';
    stream << std::setfill('0') << std::setw(2) << std::hex << (int) addr[5];
    std::string str = stream.str();
    for (int i = 0; i < MAC_ADDRESS_STRING_LENGTH; i++) {
        buff[i] = str[i];
    }
}