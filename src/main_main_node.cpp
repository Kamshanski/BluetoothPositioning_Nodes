#include "Arduino.h"
#include <Net.h>

//TODO: устранить текующую ошибку работы,
//  доделать отправку и сохранение на минорах

// GLOBALS
Net* net;

void setup() {
    Serial.begin(115200);
    net = new Net();
    Serial.println("Device started");
}
 

void loop() {
}
