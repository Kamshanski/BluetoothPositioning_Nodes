#include "Arduino.h"
#include <Node.h>

//TODO: устранить текующую ошибку работы,
//  доделать отправку и сохранение на минорах

// GLOBALS
MainNode* net;

void setup() {
    Serial.begin(115200);
    prl("a1");
    net = new MainNode();
    prl("a2");
    Serial.println("Device started");
}
 

void loop() {
}
