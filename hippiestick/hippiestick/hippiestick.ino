// 
// hippiestick 
//
// BLE-controlled LPD8806

#include "Arduino.h"
#include "Adafruit_BLE_UART.h"
#include "LPD8806.h"
#include "SPI.h"
#include "CmdMessenger.h"
#include "SharedLibrary.h"

#include "ColorfulBouncyBallPhysics.h"
#include "SimpleAnimations.h"

int g_updateInterval = 1;
int g_brightness = 1.0;
int g_currentMode = 0;
int g_logHeartbeatInterval = 3000;

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
void logBLE();
void updateSimpleAnimationsParams();
void loopStrip();

void onUnknownCommand();
void onSetMode();
void onSetUpdateInterval();
void onSetSimpleAnimationsParam1();
void onSetSimpleAnimationsParam2();
void onSetSimpleAnimationsParam3();


void setup()
{
    Serial.begin(9600);
    while(!Serial); // Leonardo/Micro should wait for serial init
    Serial.println(F("Adafruit Bluefruit Low Energy nRF8001 Print echo demo"));

    bleSerial.begin();

    LPD8806 strip = LPD8806(ledCount, 4, 5);
    strip.begin();

    modeBouncyBallPhysics.setStrip(strip);
    modeSimpleAnimations.setStrip(strip);

    // initial setup

    g_currentMode = MODE_RAINBOW_CYCLE;
    modeSimpleAnimations.setAnimationMode(MODE_RAINBOW_CYCLE);
    modeSimpleAnimations.setColor(strip.Color(255, 0, 255));

//    strip.show();

    cmdMessenger.printLfCr();
    attachCommandCallbacks();
}

void loop()
{
    pollMicrophoneLevel();
    pollPotentiometers();

//    logInputs();
    updateSimpleAnimationsParams(); // dont forget to disable this on full BLE!

    pollBluetoothStatus();
    logBLE();


// wrap looping functions in a wait using CMD_SET_UPDATE_INTERVAL

    loopStrip();
    logHeartbeat();
}

void attachCommandCallbacks()
{
    cmdMessenger.attach(onUnknownCommand);
    cmdMessenger.attach(CMD_SET_MODE, onSetMode);
//    CmdMessenger.attach(CMD_SET_UPDATE_INTERVAL, onSetUpdateInterval);
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


void logBLE()
{
    // Lets see if there's any data for us!
    if (bleSerial.available()) {
      Serial.print("* "); Serial.print(bleSerial.available()); Serial.println(F(" bytes available from BTLE"));
    }
    // OK while we still have something to read, get a character and print it out
    while (bleSerial.available()) {
      char c = bleSerial.read();
      Serial.print(c);
    }

    // Next up, see if we have any data to get from the Serial console

    if (Serial.available()) {
      // Read a line from Serial
      Serial.setTimeout(100); // 100 millisecond timeout
      String s = Serial.readString();

      // We need to convert the line to bytes, no more than 20 at this time
      uint8_t sendbuffer[20];
      s.getBytes(sendbuffer, 20);
      char sendbuffersize = min(20, s.length());

      Serial.print(F("\n* Sending -> \"")); Serial.print((char *)sendbuffer); Serial.println("\"");

      // write the data
      bleSerial.write(sendbuffer, sendbuffersize);
    }
}

void updateSimpleAnimationsParams()
{
    modeSimpleAnimations.setParam1(g_param1);
    modeSimpleAnimations.setParam2(g_param2);
    modeSimpleAnimations.setParam3(g_param3);
}

void loopStrip()
{
    if (g_currentMode == MODE_COLORFUL_MULTI_BOUNCY_BALL_PHYSICS) {
        modeBouncyBallPhysics.loop();
    } else {
        modeSimpleAnimations.loop();
    }
}

#pragma mark - Command Callbacks


void onUnknownCommand()
{
    Serial.println("received unknown command");
}

void onSetMode()
{
    Serial.print("setting mode: ");
    animation_mode_t mode = (animation_mode_t)cmdMessenger.readInt16Arg();

    if (g_currentMode != mode) {
        Serial.println(mode);

//      blank the strip
//      for(uint8_t i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);
//      strip.show();

        modeSimpleAnimations.setAnimationMode(mode);

        g_currentMode = mode;
    }
}

void onSetUpdateInterval()
{
    Serial.print("setting update interval: ");
    g_updateInterval = (int)cmdMessenger.readInt16Arg();
    Serial.println(g_updateInterval);
}

void onSetSimpleAnimationsParam1()
{
    int param1 = (int)cmdMessenger.readInt16Arg();
    modeSimpleAnimations.setParam1(param1);
}

void onSetSimpleAnimationsParam2()
{
    int param2 = (int)cmdMessenger.readInt16Arg();
    modeSimpleAnimations.setParam2(param2);
}

void onSetSimpleAnimationsParam3()
{
    int param3 = (int)cmdMessenger.readInt16Arg();
    modeSimpleAnimations.setParam3(param3);
}



