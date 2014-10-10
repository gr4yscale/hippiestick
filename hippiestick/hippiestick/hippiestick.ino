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
#include "SimpleAnimations.h"

int g_updateInterval = 1;
int g_brightness = 1.0;

aci_evt_opcode_t ble_laststatus = ACI_EVT_DISCONNECTED;

Adafruit_BLE_UART bleSerial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);
CmdMessenger cmdMessenger = CmdMessenger(bleSerial, ',', ';');

ColorfulBouncyBallPhysics modeBouncyBallPhysics;

void attachCommandCallbacks();
void pollBluetoothStatus(void);
void OnSetLed();


void setup()
{
    Serial.begin(9600);
    while(!Serial); // Leonardo/Micro should wait for serial init
    Serial.println(F("Adafruit Bluefruit Low Energy nRF8001 Print echo demo"));

    bleSerial.begin();

    LPD8806 strip = LPD8806(ledCount);
    strip.begin();
    modeBouncyBallPhysics.updateStrip(strip);

    Serial.println("strip begun");
    strip.show();

    Serial.println("strip should be shown");

    // Adds newline to every command
    cmdMessenger.printLfCr();
    attachCommandCallbacks();
}

void loop()
{
    pollBluetoothStatus();
    modeBouncyBallPhysics.loop();

//    blink(13, 1, 20);
//    delay(10);

}


//

void attachCommandCallbacks()
{
    cmdMessenger.attach(kSetLed, OnSetLed);
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


//    // Lets see if there's any data for us!
//    if (bleSerial.available()) {
//      Serial.print("* "); Serial.print(bleSerial.available()); Serial.println(F(" bytes available from BTLE"));
//    }
//    // OK while we still have something to read, get a character and print it out
//    while (bleSerial.available()) {
//      char c = bleSerial.read();
//      Serial.print(c);
//    }
//
//    // Next up, see if we have any data to get from the Serial console
//
//    if (Serial.available()) {
//      // Read a line from Serial
//      Serial.setTimeout(100); // 100 millisecond timeout
//      String s = Serial.readString();
//
//      // We need to convert the line to bytes, no more than 20 at this time
//      uint8_t sendbuffer[20];
//      s.getBytes(sendbuffer, 20);
//      char sendbuffersize = min(20, s.length());
//
//      Serial.print(F("\n* Sending -> \"")); Serial.print((char *)sendbuffer); Serial.println("\"");
//
//      // write the data
//      bleSerial.write(sendbuffer, sendbuffersize);
//    }
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
