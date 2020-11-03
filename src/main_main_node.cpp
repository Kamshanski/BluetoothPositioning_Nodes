#include "Arduino.h"
#include <Node.h>

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define KBite 1024

// Почитать: http://www.blesstags.eu/2017/10/reading-notifying.html 

// Late TODO: настройить отправку RSSI c учётом того, что скан блокирует полностью потоки. Либо останавливать скан, отправлять и возвращать скан. Либо сделать внутри отправку другим способом. Нотификацией


//TODO: проверить неправильность поиска адреса сосканированного устройства в targetsSet

#define MAIN new MainNode("Main_node", 37)
// #define SLAVE_1 new SlaveNode("Slave_node_1", 41)
// #define SLAVE_2 new SlaveNode("Slave_node_2", 42)
// #define SLAVE_3 new SlaveNode("Slave_node_3", 43)
// #define SLAVE_4 new SlaveNode("Slave_node_4", 44)



// GLOBALS
#ifdef MAIN
MainNode* node;
#else
SlaveNode* node;
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

void TaskScan(void *pvParameters) {
    (void) pvParameters;

    Serial.println("init SCAN Task...");
    node->initScanTool();
    node->scan(0);
    Serial.println("... SCAN Task init DONE.");

    while (true) {              // Eternal task of scanning
        // if (node->targetsSet->getSize() < 1 && node->isScanning()) {
        //     node->scanStop();
        // }
        // else if (node->targetsSet->getSize() > 0 && !node->isScanning()) {
        //     Serial.println("Scan started");
        //     node->scan(0);
        // }
        // vTaskDelay(10 / portTICK_RATE_MS);
    }                             
}

void TaskWrite(void *pvParameters)  {
    (void) pvParameters;
#ifndef MAIN
    node->xSemaphore = xSemaphoreCreateMutex();
    Serial.println("connecting");
    node->connectToMainNode();                        // Connect to MainNode instantly
    Serial.println("connected");
    node->syncTargetsSet();                           // Synchronize collected data
    Serial.println("synchronized");
#endif

    while (true) {
        #ifndef MAIN
        if (node->hasRssiDiscovered()) {  
            node->sendToMain();
        }
        
        #endif
        vTaskDelay(30 / portTICK_RATE_MS);
    }
}


void setup() {
    Serial.begin(115200);
    
    setNodeWithPreprocessor();
    prl("device inited");

    xTaskCreatePinnedToCore(
        TaskWrite,
        "AnalogReadA3",
        KBite*10,  // Stack size
        NULL,
        2,  // Priority
        NULL, 
        ARDUINO_RUNNING_CORE
    );

    delay(350);

    xTaskCreatePinnedToCore(
        TaskScan, 
        "TaskBlink",   // A name just for humans
        KBite*3,  // This stack size can be checked & adjusted by reading the Stack Highwater
        NULL,
        1,  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        NULL, 
        ARDUINO_RUNNING_CORE
    );
    
    Serial.println("Device started");
}


void loop() {
}
