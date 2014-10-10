// 
// hippiestick 
//
// BLE-controlled LPD8806

#include "Arduino.h"
#include "Adafruit_BLE_UART.h"
#include "LPD8806.h"
#include "CmdMessenger.h"
#include "SharedLibrary.h"

//#include "a_colorful_multi_bouncy_ball_physics.h"

#include "ColorfulBouncyBallPhysics.h"

aci_evt_opcode_t ble_laststatus = ACI_EVT_DISCONNECTED;

Adafruit_BLE_UART bleSerial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);
LPD8806 strip = LPD8806(ledCount);
CmdMessenger cmdMessenger = CmdMessenger(bleSerial, ',', ';');


void attachCommandCallbacks();
void pollBluetoothStatus(void);
void OnSetLed();


void setup()
{
    pinMode(13, OUTPUT);

    Serial.begin(9600);
    while(!Serial); // Leonardo/Micro should wait for serial init
    Serial.println(F("Adafruit Bluefruit Low Energy nRF8001 Print echo demo"));

    strip.begin();

    Serial.println("strip begun");

    strip.show();

    Serial.println("strip should be shown");

    // Adds newline to every command
    cmdMessenger.printLfCr();
    attachCommandCallbacks();
}

void loop()
{
    blink(13, 1, 20);
    delay(10);
}

//

void attachCommandCallbacks()
{
    cmdMessenger.attach(kSetLed, OnSetLed);
    Serial.println("attached on set led callback");
}

void pollBluetoothStatus(void)
{
    bleSerial.pollACI();     // Tell the nRF8001 to do whatever it should be working on.

    aci_evt_opcode_t ble_current_status = bleSerial.getState();

    if (ble_current_status != ble_laststatus) {
        if (ble_current_status == ACI_EVT_DEVICE_STARTED) {
            Serial.println(F("* Advertising started"));
        }
        if (ble_current_status == ACI_EVT_CONNECTED) {
            Serial.println(F("* Connected!"));
        }
        if (ble_current_status == ACI_EVT_DISCONNECTED) {
            Serial.println(F("* Disconnected or advertising timed out"));
        }
        ble_laststatus = ble_current_status;
    }

    if (ble_current_status == ACI_EVT_CONNECTED) {
        cmdMessenger.feedinSerialData();
    }
}

// Command callbacks

void OnSetLed()
{
//    addedForce = cmdMessenger.readInt16Arg();
//    gravity = cmdMessenger.readFloatArg();

    // Set led
    Serial.println("changing gravity:");
//    Serial.println(gravity, 4);
    //  digitalWrite(kBlinkLed, ledState?HIGH:LOW);
}
