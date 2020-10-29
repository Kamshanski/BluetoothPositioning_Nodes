#include "Arduino.h"
#include <Node.h>

// Late TODO: сихнронизация SlaveNode относительно MainNode при подключении первого ко второму

//TODO: проверить неправильность поиска адреса сосканированного устройства в targetsSet

// #define MAIN new MainNode("Main_node")
#define SLAVE_1 new SlaveNode("Slave_node_1")
// #define SLAVE_2 new SlaveNode("Slave_node_2")
// #define SLAVE_3 new SlaveNode("Slave_node_3")
// #define SLAVE_4 new SlaveNode("Slave_node_4")


// GLOBALS
#ifdef MAIN
MainNode* node;
#else
#if defined(SLAVE_1) || defined(SLAVE_2) || defined(SLAVE_3) || defined(SLAVE_4)
SlaveNode* node;
#endif
#endif

 void setNodeWithPreprocessor() {
#ifdef MAIN
    node = MAIN;
#endif
#ifdef SLAVE_1
    node = SLAVE_1;
#endif
#ifdef SLAVE_2
    node = SLAVE_2;
#endif
#ifdef SLAVE_3
    node = SLAVE_3;
#endif
#ifdef SLAVE_4
    node = SLAVE_4;
#endif   
 }

void setup() {
    Serial.begin(115200);
    Serial.println("Device started");
    setNodeWithPreprocessor();
    Serial.println("Device is on!");
}

void loop() {
}
