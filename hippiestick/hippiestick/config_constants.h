#include "Arduino.h"

#ifndef config_constants_h
#define config_constants_h

const int ledCount = 208;

#define ADAFRUITBLE_REQ 10
#define ADAFRUITBLE_RDY 3
#define ADAFRUITBLE_RST 9

enum
{
    kSetLed, // Command to request led to be set in specific state
};

#endif
