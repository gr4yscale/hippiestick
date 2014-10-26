#include "Arduino.h"

#ifndef SimpleAnimations_h
#define SimpleAnimations_h

#include "LPD8806.h"
#include "SharedLibrary.h"

class SimpleAnimations {

    // for randomColors animation
    long randPixel;
    long offsetR;
    long offsetG;
    long offsetB;
    long randR;
    long randG;
    long randB;

    unsigned long timeStamp;
    unsigned long stopTime;

public:
    SimpleAnimations(void);
    void setStrip(LPD8806 s);
    void setAnimationMode(animation_mode_t am);
    void setColor(uint32_t c);
    void setCycles(int cyc);
    void loop();

private:
    LPD8806 strip;
    animation_mode_t animationMode;
    uint32_t color;
    int cycles;

    const int sampleWindow = 50;
    unsigned int sample;

    void colorWipe(uint32_t c);
    void dither(uint32_t c);
    void scanner(uint8_t r, uint8_t g, uint8_t b);
    void wave(uint32_t c, int cycles);
    void wave2(uint32_t c, int cycles);
    void rainbowCycle();
    void uniformBreathe(uint8_t* breatheTable, uint8_t breatheTableSize, uint16_t updatePeriod, uint16_t r, uint16_t g, uint16_t b);
    void sequencedBreathe(uint8_t* breatheTable, uint8_t breatheTableSize, uint16_t updatePeriod, uint16_t r, uint16_t g, uint16_t b);
    void randomColors();
    void microPhoneLevel();

    uint32_t Wheel(uint16_t WheelPos);
};


#endif
