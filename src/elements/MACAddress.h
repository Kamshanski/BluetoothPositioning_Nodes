#pragma once


#include <esp_bt_defs.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <string.h>
#include <stdio.h>

/**
 * Wrapper of MAC adress. 
 */
class MACAddress {
private:
    esp_bd_addr_t addr;
    std::string *addrString;
    bool hasString = false;
public:
    MACAddress();
    MACAddress(esp_bd_addr_t const &addr);

    void set(esp_bd_addr_t const &addr);
    
    std::string *toString();
    bool equals(MACAddress* another);
    ~MACAddress();
    inline bool operator< (const MACAddress& another)
    {
        for (int i = 0; i < ESP_BD_ADDR_LEN; i++) {
            if (addr[i] < another.addr[i]) {
                return true;
            } 
            else if (addr[i] > another.addr[i]) {
                return false;
            }
        }
    }
};
