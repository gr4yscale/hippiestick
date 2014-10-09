// 
// hippiestick 
//
// BLE-controlled LPD8806

#include "Arduino.h"
#include "SPI.h"

// Include application, user and local libraries
#include "LocalLibrary.h"


// Define variables and constants

uint8_t myLED;


void setup() {
    myLED = 13;
    pinMode(myLED, OUTPUT);
}

void loop() {
    blink(myLED, 1, 20);
    delay(10);
}
