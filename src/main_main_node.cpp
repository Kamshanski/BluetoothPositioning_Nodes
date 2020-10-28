#include "Arduino.h"
#include <Node.h>

//TODO: доделать отправку и сохранение на минорах

// GLOBALS
MainNode* mainNode;

void setup() {
    Serial.begin(115200);
    mainNode = new MainNode("slaveNode1");
    Serial.println("Device started");
}
 

void loop() {
}
