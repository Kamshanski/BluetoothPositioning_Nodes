#include "AddressSet.h"  
#include "utilities.h"
#include "HardwareSerial.h"
#include <MACAddresses.h>

AddressSet::AddressSet() : AddressSet::AddressSet(10) {}

AddressSet::AddressSet(int initialCapacity) {
    capacity = initialCapacity;
    targets = new uint8_t[ESP_BD_ADDR_LEN*capacity];
}

AddressSet::~AddressSet() {
    delete targets;
}

int AddressSet::addOrRemove(uint8_t addr[ESP_BD_ADDR_LEN]) {
    int pos = find(addr);
    if (pos < 0) {
        add(addr);
        return NEW_DEVICE_WAS_ADDED;
    } else {
        remove(pos);
        return DEVICE_WAS_REMOVED;
    }
}
/**
 * Returns position of address in array it it's present. Otherwise returns -1
*/
int AddressSet::find(uint8_t *addr) {
    for (int i = 0; i < size; i++) {
        bool equal = true;
        int addrIndex = ESP_BD_ADDR_LEN * i;
        for (int j = 0; (j < ESP_BD_ADDR_LEN) & (equal == true); j++) {
            equal = targets[addrIndex + j] == addr[j];
        }
        if (equal == true) {
            return i;
        }        
    }
    return -1;
}

void AddressSet::add(uint8_t *addr) {
    if (size == capacity) {
        capacity+=10;
        resize(capacity);
    }

    int pos = size*ESP_BD_ADDR_LEN;
    for (int i = 0; i < ESP_BD_ADDR_LEN; i++) {
        targets[pos++] = addr[i];
    }
    size++;

}

void AddressSet::remove(uint8_t *addr) {
    int pos = find(addr);
    remove(pos);
}

void AddressSet::remove(int pos) {
    int newPos = pos * ESP_BD_ADDR_LEN;
    int lastPos = (size - 1) * ESP_BD_ADDR_LEN;
    for (int i = 0; i < ESP_BD_ADDR_LEN; i++) {
        targets[newPos + i] = targets[lastPos + i];
    }
    size--;
}

std::string AddressSet::getAddress(int pos) {
    char addr[MAC_ADDRESS_STRING_LENGTH];
    getAddrString(pos, addr);
    return std::string(addr);
}

int AddressSet::getSize() {
    return size;
}

std::string AddressSet::toString() {
    std::stringstream stream;
    stream << "Targets{"
           << "\nSize" << size
           << "\nCapacity" << capacity
           << "\nDeviceMAC";
    for (int i = 0; i < size; i++) {
        char addr[MAC_ADDRESS_STRING_LENGTH];
        getAddrString(i, addr);
        for (int i = 0; i < MAC_ADDRESS_STRING_LENGTH; i++) { // This loop solve the problem of extra characters in the end of addr (e.g. 5c:ea:6a:1d:48:42␄MA?)
            stream << addr[i];
        }
    }
    stream << "\n}Targets";
    return stream.str();
}

void AddressSet::resize(int newCapacity) {
    uint8_t * newTargetsArray = new uint8_t[newCapacity*ESP_BD_ADDR_LEN];
    int limit = (newCapacity > capacity ? capacity : newCapacity) * ESP_BD_ADDR_LEN;
    for (size_t i = 0; i < limit; i++) {
        newTargetsArray[i] = targets[i];
    }
    delete targets;
    targets = newTargetsArray;
}

void AddressSet::getAddrString(int pos, char * buff) {
    pos = pos * ESP_BD_ADDR_LEN;
    addrToString(targets + pos, buff);
}



