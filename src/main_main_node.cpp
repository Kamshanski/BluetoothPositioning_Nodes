#include "Arduino.h"
#include <Net.h>

//TODO: проверить добавление и удаление телефона при продлючениях, 
//  узнать МАС-адреса миноров и гланого, 
//  сделать отправку и сохранение на минорах

// GLOBALS
Net* net;

void setup() {
    Serial.begin(115200);
    net = new Net();
    Serial.println("Device started");
}
 

void loop() {
}
