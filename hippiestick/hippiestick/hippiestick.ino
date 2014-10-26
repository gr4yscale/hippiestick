// 
// hippiestick 
//
// BLE-controlled LPD8806

#include "Arduino.h"
#include "Adafruit_BLE_UART.h"
#include "LPD8806.h"
#include "CmdMessenger.h"
#include "SharedLibrary.h"

#include "ColorfulBouncyBallPhysics.h"
#include "SimpleAnimations.h"

int g_updateInterval = 1;
int g_brightness = 1.0;
int g_currentMode = 0;
int g_logHeartbeatInterval = 1000;

aci_evt_opcode_t ble_laststatus = ACI_EVT_DISCONNECTED;

Adafruit_BLE_UART bleSerial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);
CmdMessenger cmdMessenger = CmdMessenger(bleSerial, ',', ';');

ColorfulBouncyBallPhysics modeBouncyBallPhysics;
SimpleAnimations modeSimpleAnimations;

void attachCommandCallbacks();
void pollBluetoothStatus(void);
void logHeartbeat();
void onUnknownCommand();
void onSetMode();
void onSetModeParamSimpleAnimationsColor();
void onSetModeParamSimpleAnimationsCycles();


void setup()
{
    Serial.begin(9600);
    while(!Serial); // Leonardo/Micro should wait for serial init
    Serial.println(F("Adafruit Bluefruit Low Energy nRF8001 Print echo demo"));

    bleSerial.begin();

    LPD8806 strip = LPD8806(ledCount);
    strip.begin();

    // sets the strip for bouncy ball physics mode; strip not avail when we declare it (needs to be global)
//    modeBouncyBallPhysics.setStrip(strip);
    modeSimpleAnimations.setStrip(strip);

    // initial setup

    modeSimpleAnimations.setAnimationMode(MODE_AUDIO_REACTIVE_WAVE);
    modeSimpleAnimations.setColor(strip.Color(10,100,0));

    strip.show();


    // Adds newline to every command
//    cmdMessenger.printLfCr();
//    attachCommandCallbacks();


}

void loop()
{


    modeSimpleAnimations.loop();

//    pollBluetoothStatus();

    // wrap looping functions in a wait using CMD_SET_UPDATE_INTERVAL


//    switch (g_currentMode) {
//        case MODE_COLORFUL_MULTI_BOUNCY_BALL_PHYSICS:
//            modeBouncyBallPhysics.loop();
//            break;
//        case MODE_COLOR_WIPE:
//        case MODE_DITHER: // scanner
//        case MODE_WAVE:
//        case MODE_WAVE2:
//        case MODE_RAINBOW_CYCLE:
//        case MODE_RANDOM_COLORS:
//            Serial.println("calling simpleAnimations loop");
//            modeSimpleAnimations.loop();
//            break;
//
//        default:
//            modeBouncyBallPhysics.loop();
//            break;
//    }

    // need a heartbeat

//    logHeartbeat();
}

void attachCommandCallbacks()
{
    cmdMessenger.attach(CMD_SET_MODE, onSetMode);
    cmdMessenger.attach(CMD_SET_MODE_PARAM_SIMPLE_ANIMATIONS_COLOR, onSetModeParamSimpleAnimationsColor);
    cmdMessenger.attach(CMD_SET_MODE_PARAM_SIMPLE_ANIMATIONS_CYCLES, onSetModeParamSimpleAnimationsCycles);
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

void logHeartbeat()
{
    static long nextStep = 0;
    long milliSeconds = millis();

    if (milliSeconds >= nextStep) {
        nextStep += g_logHeartbeatInterval;
        Serial.println("I'm alive!");
    }
}

// Command Callbacks


void onUnknownCommand()
{
    Serial.println("received unknown command");
}

void onSetMode()
{
    Serial.println("Going to set the mode now");
    animation_mode_t mode = (animation_mode_t)cmdMessenger.readInt16Arg();

    if (g_currentMode != mode) {
        Serial.println("changing mode: ");
        Serial.print(mode);

//      blank the strip
//      for(uint8_t i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);
//      strip.show();

        modeSimpleAnimations.setAnimationMode(mode);

        g_currentMode = mode;
    }
}

void onSetModeParamSimpleAnimationsColor()
{
    // update an instance variable on the to-be0converted-to-a-class SimpleAnimations class library

}

void onSetModeParamSimpleAnimationsCycles()
{

    
}