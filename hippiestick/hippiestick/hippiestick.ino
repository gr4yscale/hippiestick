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

int g_paramMicrophoneLevel = 0;
int g_param1 = 0;
int g_param2 = 0;
int g_param3 = 0;
int g_param4 = 0;
int g_param5 = 0;

aci_evt_opcode_t ble_laststatus = ACI_EVT_DISCONNECTED;

Adafruit_BLE_UART bleSerial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);
CmdMessenger cmdMessenger = CmdMessenger(bleSerial, ',', ';');

ColorfulBouncyBallPhysics modeBouncyBallPhysics;
SimpleAnimations modeSimpleAnimations;

void attachCommandCallbacks();
void pollBluetoothStatus(void);
void pollMicrophoneLevel();
void pollPotentiometers();
void logHeartbeat();
void logInputs();
void onUnknownCommand();
void onSetMode();
void updateSimpleAnimationsParams();


void setup()
{
    Serial.begin(9600);
    while(!Serial); // Leonardo/Micro should wait for serial init
    Serial.println(F("Adafruit Bluefruit Low Energy nRF8001 Print echo demo"));

    bleSerial.begin();

    LPD8806 strip = LPD8806(ledCount);
    strip.begin();

    modeBouncyBallPhysics.setStrip(strip);
    modeSimpleAnimations.setStrip(strip);

    // initial setup

    modeSimpleAnimations.setAnimationMode(MODE_MICROPHONE_LEVEL);

    strip.show();


    // Adds newline to every command
//    cmdMessenger.printLfCr();
//    attachCommandCallbacks();
}

void loop()
{
    pollMicrophoneLevel();
    pollPotentiometers();
//    logInputs();

    updateSimpleAnimationsParams();

//    pollBluetoothStatus();

    modeSimpleAnimations.loop();
//    modeBouncyBallPhysics.loop();

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

//    logHeartbeat();

}

void attachCommandCallbacks()
{
    cmdMessenger.attach(CMD_SET_MODE, onSetMode);
//    cmdMessenger.attach(CMD_SET_MODE_PARAM_SIMPLE_ANIMATIONS_COLOR, onSetModeParamSimpleAnimationsColor);
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

void pollMicrophoneLevel()
{
    g_paramMicrophoneLevel = analogRead(0);
}

void pollPotentiometers()
{
    g_param1 = analogRead(1);
    g_param2 = analogRead(2);
    g_param3 = analogRead(3);
    g_param4 = analogRead(4);
    g_param5 = analogRead(5);
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

void logInputs()
{
    Serial.print("param mic level: ");
    Serial.print(g_paramMicrophoneLevel);
    Serial.print(" | ");

    Serial.print("param 1: ");
    Serial.print(g_param1);
    Serial.print(" | ");

    Serial.print("param 2: ");
    Serial.print(g_param2);
    Serial.print(" | ");

    Serial.print("param 3: ");
    Serial.print(g_param3);
    Serial.print(" | ");

    Serial.print("param 4: ");
    Serial.print(g_param4);
    Serial.print(" | ");

    Serial.print("param 5: ");
    Serial.print(g_param5);

    Serial.println(" ");
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

void updateSimpleAnimationsParams()
{
    modeSimpleAnimations.setParam1(g_param1);
    modeSimpleAnimations.setParam2(g_param2);
    modeSimpleAnimations.setParam3(g_param3);
}