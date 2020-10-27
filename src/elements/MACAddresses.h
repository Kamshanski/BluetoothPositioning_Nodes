#pragma once
#include <string>
#include <BLEAddress.h>

// MAIN
static BLEAddress* const MAIN_ADDR = new BLEAddress(std::string("B4:E6:2D:C1:E5:07"));

// SLAVES
#define SLAVES_NUMBER  4
static BLEAddress* const SLAVES_ADDR[SLAVES_NUMBER] = { 
    new BLEAddress(std::string("A4:CF:12:8D:40:BE")),
    new BLEAddress(std::string("24:6F:28:B2:DC:7A")),
    new BLEAddress(std::string("A4:CF:12:8D:75:C2")),
    new BLEAddress(std::string("30:AE:A4:8B:44:2A"))
};

// SERVER
//  Добавить сервер, который будет всё считать
