// 
// hippiestick 
//
// BLE-controlled LPD8806

#include "Arduino.h"
#include "LocalLibrary.h"
#include "config_constants.h"
#include "Adafruit_BLE_UART.h"
#include "LPD8806.h"
#include "CmdMessenger.h"

uint8_t myLED;

void setup() {
    myLED = 13;
    pinMode(myLED, OUTPUT);

    Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);

    LPD8806 strip = LPD8806(208);

    CmdMessenger cmdMessenger = CmdMessenger(BTLEserial, ',', ';');
}

void loop() {
    blink(myLED, 1, 20);
    delay(10);
}
