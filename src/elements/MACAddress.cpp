#include <MACAddress.h>

MACAddress::MACAddress() {}

MACAddress::MACAddress(esp_bd_addr_t const &addr) { 
    set(addr);
}

void MACAddress::set(esp_bd_addr_t const &addr) {
    for (int i = 0; i < ESP_BD_ADDR_LEN; i++) {
        this->addr[i] = addr[i];
    }
}

std::string* MACAddress::toString() {
    if (!hasString) {
        std::stringstream stream;
        stream << std::hex << (int) ((uint8_t*) addr)[0] << ':';
        stream << std::hex << (int) ((uint8_t*) addr)[1] << ':';
        stream << std::hex << (int) ((uint8_t*) addr)[2] << ':';
        stream << std::hex << (int) ((uint8_t*) addr)[3] << ':';
        stream << std::hex << (int) ((uint8_t*) addr)[4] << ':';
        stream << std::hex << (int) ((uint8_t*) addr)[5];
        *addrString = stream.str();
    }
    return addrString;
}

bool MACAddress::equals(MACAddress* another) {
    for (int i = 0; i < ESP_BD_ADDR_LEN; i++) {
        if (addr[i] != another->addr[i]) {
            return false;
        }
    }
    return true;
}

MACAddress::~MACAddress() {
    delete addrString;
}
