#include "Arduino.h"

#ifndef SharedLibrary_h
#define SharedLibrary_h

const int ledCount = 208;

#define ADAFRUITBLE_REQ 10
#define ADAFRUITBLE_RDY 3
#define ADAFRUITBLE_RST 9

typedef enum
{
    CMD_SET_MODE                        = 1,
    CMD_SET_UPDATE_INTERVAL             = 2,
    CMD_SET_BRIGHTNESS                  = 3,
    CMD_SET_SIMPLE_ANIMATIONS_PARAM1    = 4,
    CMD_SET_SIMPLE_ANIMATIONS_PARAM2    = 5,
    CMD_SET_SIMPLE_ANIMATIONS_PARAM3    = 6
} cmd_t;

typedef enum
{
    MODE_COLORFUL_MULTI_BOUNCY_BALL_PHYSICS         = 0,
    MODE_COLOR_WIPE                                 = 1,
    MODE_DITHER                                     = 2,
    MODE_SCANNER                                    = 3,
    MODE_WAVE                                       = 4,
    MODE_WAVE2                                      = 5,
    MODE_RAINBOW_CYCLE                              = 6,
    MODE_UNIFORM_BREATHE                            = 7,
    MODE_SEQUENCED_BREATHE                          = 8,
    MODE_RANDOM_COLORS                              = 9,
    MODE_MICROPHONE_LEVEL                           = 10
} animation_mode_t;

#endif
