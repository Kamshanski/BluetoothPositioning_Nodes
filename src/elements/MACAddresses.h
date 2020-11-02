#pragma once
#include <string>
#include <BLEAddress.h>

static const int MAC_ADDRESS_STRING_LENGTH = 6*2+5;

// MAIN
static BLEAddress* const MAIN_ADDR = new BLEAddress(std::string("B4:E6:2D:C1:E5:07"));

// SLAVES
#define SLAVES_NUMBER  3
static BLEAddress* const SLAVES_ADDR[SLAVES_NUMBER] = { 
    new BLEAddress(std::string("a4:cf:12:8d:40:be")),
//    new BLEAddress(std::string("24:6f:28:b2:dc:7a")),
    new BLEAddress(std::string("a4:cf:12:8d:75:c2")),
    new BLEAddress(std::string("30:ae:a4:8b:44:2a"))
};

bool isAnyOfSlaves(uint8_t * addr);
void copyAddress(uint8_t * from, uint8_t * to);

void addrToString(uint8_t * addr, char * buff);
// SERVER
//  Добавить сервер, который будет всё подсчитывать
