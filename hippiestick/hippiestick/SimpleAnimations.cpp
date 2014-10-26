#include "SimpleAnimations.h"

uint8_t LED_Breathe_Table[]  = {   80,  87,  95, 103, 112, 121, 131, 141, 151, 161, 172, 182, 192, 202, 211, 220,
    228, 236, 242, 247, 251, 254, 255, 255, 254, 251, 247, 242, 236, 228, 220, 211, 202, 192, 182, 172, 161, 151,
    141, 131, 121, 112, 103,  95,  87,  80,  73,  66,  60,  55,  50,  45,  41,  38,  34,  31,  28,  26,  24,  22,
    20,  18,  16,  14,  12,  10,   8,   6,   4,   2,   0,   0,   0,   0,   0,   0,   0,   0,   2,   4,   6,   8,
    10,  12,  14,  16,  18,  20,  22,  24,  26,  28,  31,  34,  38,  41,  45,  50,  55,  60,  66,  73 };

#define BREATHE_TABLE_SIZE (sizeof(LED_Breathe_Table))
#define BREATHE_CYCLE    6000      /*breathe cycle in milliseconds*/
#define BREATHE_UPDATE    (BREATHE_CYCLE / BREATHE_TABLE_SIZE)
#define PI 3.14159265

SimpleAnimations::SimpleAnimations(void) {}

void SimpleAnimations::setStrip(LPD8806 s)
{
    strip = s;
}

void SimpleAnimations::setAnimationMode(animation_mode_t am)
{
    animationMode = am;
}

void SimpleAnimations::setColor(uint32_t c)
{
    color = c;
}

void SimpleAnimations::setCycles(int cyc)
{
    cycles = cyc;
}

void SimpleAnimations::loop()
{
    switch (animationMode) {
        case MODE_COLOR_WIPE:
            colorWipe(color);
            break;
        case MODE_DITHER:
            dither(color);
            break;              // scanner
        case MODE_WAVE:
            wave(color, cycles);
            break;
        case MODE_WAVE2:
            wave2(color, cycles);
            break;
        case MODE_RAINBOW_CYCLE:
            rainbowCycle();
            break;              // uniform breathe, sequenced breathe
        case MODE_RANDOM_COLORS:
            randomColors();
            break;
        case MODE_MICROPHONE_LEVEL:
            microPhoneLevel();
            break;
        default:
            randomColors();
            break;
    }
    Serial.println("looping");

}

// fill the dots one after the other with said color

void SimpleAnimations::colorWipe(uint32_t c)
{
    Serial.println("i should set some colorz");
    int i;
    for (i=0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
        strip.show();
        Serial.println("i just set some colorz");
    }
}

// An "ordered dither" fills every pixel in a sequence that looks
// sparkly and almost random, but actually follows a specific order.

void SimpleAnimations::dither(uint32_t c)
{
    // Determine highest bit needed to represent pixel index
    int hiBit = 0;
    int n = strip.numPixels() - 1;
    for(int bit=1; bit < 0x8000; bit <<= 1) {

        if(n & bit) hiBit = bit;
    }

    int bit, reverse;
    for(int i=0; i<(hiBit << 1); i++) {
        // Reverse the bits in i to create ordered dither:
        reverse = 0;
        for(bit=1; bit <= hiBit; bit <<= 1) {
            reverse <<= 1;
            if(i & bit) reverse |= 1;
        }
        strip.setPixelColor(reverse, c);
        digitalWrite(ADAFRUITBLE_REQ, 1);
        strip.show();
        digitalWrite(ADAFRUITBLE_REQ, 0);
    }
}


// "Larson scanner" = Cylon/KITT bouncing light effect

void SimpleAnimations::scanner(uint8_t r, uint8_t g, uint8_t b)
{
    int i, j, pos, dir;

    pos = 0;
    dir = 1;

    for(i=0; i<((strip.numPixels()-1) * 2); i++) {
        // Draw 5 pixels centered on pos.  setPixelColor() will clip
        // any pixels off the ends of the strip, no worries there.
        // we'll make the colors dimmer at the edges for a nice pulse
        // look

        strip.setPixelColor(pos - 1, strip.Color(r, g, b));
        strip.setPixelColor(pos - 3, strip.Color(r, g, b));
        strip.setPixelColor(pos - 4, strip.Color(r, g, b));
        strip.setPixelColor(pos, strip.Color(r, g, b));
        strip.setPixelColor(pos + 1, strip.Color(r, g, b));
        strip.setPixelColor(pos + 3, strip.Color(r, g, b));
        strip.setPixelColor(pos + 4, strip.Color(r, g, b));

        digitalWrite(ADAFRUITBLE_REQ, 1);
        strip.show();
        digitalWrite(ADAFRUITBLE_REQ, 0);

        // If we wanted to be sneaky we could erase just the tail end
        // pixel, but it's much easier just to erase the whole thing
        // and draw a new one next time.
        for(j=-12; j<= 12; j++)
            strip.setPixelColor(pos+j, strip.Color(0,0,0));
        // Bounce off ends of strip
        pos += dir;
        if(pos < 0) {
            pos = 1;
            dir = -dir;
        } else if(pos >= strip.numPixels()) {
            pos = strip.numPixels() - 12;
            dir = -dir;
        }
    }
}


// Sine wave effect

void SimpleAnimations::wave(uint32_t c, int cycles)
{
    float y;
    byte  r, g, b, r2, g2, b2;

    // Need to decompose color into its r, g, b elements
    g = (c >> 16) & 0x7f;
    r = (c >>  8) & 0x7f;
    b =  c        & 0x7f;

    for(int x=0; x<(strip.numPixels()*5); x++)
    {

        for(int i=0; i<strip.numPixels(); i++) {

            y = sin(PI * (float)cycles * (float)(x + i) / (float)strip.numPixels());
            if(y >= 0.0) {
                // Peaks of sine wave are white
                y  = 1.0 - y; // Translate Y to 0.0 (top) to 1.0 (center)
                r2 = 127 - (byte)((float)(127 - r) * y);
                g2 = 127 - (byte)((float)(127 - g) * y);
                b2 = 127 - (byte)((float)(127 - b) * y);
            } else {
                // Troughs of sine wave are black
                y += 1.0; // Translate Y to 0.0 (bottom) to 1.0 (center)
                r2 = (byte)((float)r * y);
                g2 = (byte)((float)g * y);
                b2 = (byte)((float)b * y);
            }
            strip.setPixelColor(i, r2, g2, b2);
        }
        digitalWrite(ADAFRUITBLE_REQ, 1);
        strip.show();
        digitalWrite(ADAFRUITBLE_REQ, 0);
    }
}


// Sine wave effect

void SimpleAnimations::wave2(uint32_t c, int cycles)
{
    float y;
    byte  r, g, b, r2, g2, b2;

    // Need to decompose color into its r, g, b elements
    g = (c >> 16) & 0x7f;
    r = (c >>  8) & 0x7f;
    b =  c        & 0x7f;

    for(int x=0; x<(strip.numPixels()*5); x++) {
        for(int i=0; i<strip.numPixels(); i++) {
            y = sin(PI * (float)cycles * (float)(x + i) / (float)strip.numPixels());
            if(y >= 0.0) {
                // Peaks of sine wave are white
                y  = 1.0 - y; // Translate Y to 0.0 (top) to 1.0 (center)
                r2 = 0 - (byte)((float)(0 - r) * y);
                g2 = 0 - (byte)((float)(0 - g) * y);
                b2 = 127 - (byte)((float)(127 - b) * y);
            } else {
                // Troughs of sine wave are black
                y += 1.0; // Translate Y to 0.0 (bottom) to 1.0 (center)
                r2 = 50 - (byte)((float)(50 - r) * y);
                g2 = 127 - (byte)((float)(127 - g) * y);
                b2 = (byte)((float)b * y);
            }
            strip.setPixelColor(i, r2, g2, b2);
        }
        digitalWrite(ADAFRUITBLE_REQ, 1);
        strip.show();
        digitalWrite(ADAFRUITBLE_REQ, 0);
    }
}


// Cycle through the color wheel, equally spaced along strip

void SimpleAnimations::rainbowCycle()
{
    uint16_t i, j;

    for (j=0; j < 384 * 5; j++) {     // 5 cycles of all 384 colors in the wheel
        for (i=0; i < strip.numPixels(); i++) {
            // tricky math! we use each pixel as a fraction of the full 384-color
            // wheel (thats the i / strip.numPixels() part)
            // Then add in j which makes the colors go around per pixel
            // the % 384 is to make the wheel cycle around
            strip.setPixelColor(i, Wheel(((i * 384 / strip.numPixels()) + j) % 384));
        }
        digitalWrite(ADAFRUITBLE_REQ, 1);
        strip.show();   // write all the pixels out
        digitalWrite(ADAFRUITBLE_REQ, 0);
    }
}


void SimpleAnimations::uniformBreathe(uint8_t* breatheTable, uint8_t breatheTableSize, uint16_t updatePeriod, uint16_t r, uint16_t g, uint16_t b)
{
    int i;
    uint8_t breatheIndex = 0;
    uint8_t breatheRed;
    uint8_t breatheGrn;
    uint8_t breatheBlu;

    for (breatheIndex = 0; breatheIndex < breatheTableSize; breatheIndex++) {
        for (i=0; i < strip.numPixels(); i++) {
            breatheRed = (r * breatheTable[breatheIndex]) / 256;
            breatheGrn = (g * breatheTable[breatheIndex]) / 256;
            breatheBlu = (b * breatheTable[breatheIndex]) / 256;
            strip.setPixelColor(i, breatheRed, breatheGrn, breatheBlu);
        }
        digitalWrite(ADAFRUITBLE_REQ, 1);
        strip.show();   // write all the pixels out
        digitalWrite(ADAFRUITBLE_REQ, 0);
        delay(updatePeriod);
    }
}

void SimpleAnimations::sequencedBreathe(uint8_t* breatheTable, uint8_t breatheTableSize, uint16_t updatePeriod, uint16_t r, uint16_t g, uint16_t b)
{
    int i;
    uint8_t breatheIndex = 0;
    uint8_t breatheRed;
    uint8_t breatheGrn;
    uint8_t breatheBlu;
    uint8_t sequenceIndex;

    for (breatheIndex = 0; breatheIndex < breatheTableSize; breatheIndex++) {
        for (i=0; i < strip.numPixels(); i++) {
            sequenceIndex = (breatheIndex + (i*4)) % breatheTableSize;
            breatheRed = (r * breatheTable[sequenceIndex]) / 256;
            breatheGrn = (g * breatheTable[sequenceIndex]) / 256;
            breatheBlu = (b * breatheTable[sequenceIndex]) / 256;
            strip.setPixelColor(i, breatheRed, breatheGrn, breatheBlu);
        }
        digitalWrite(ADAFRUITBLE_REQ, 1);
        strip.show();
        digitalWrite(ADAFRUITBLE_REQ, 0);
        delay(updatePeriod);
    }
}


// Random Colors, one at a time

void SimpleAnimations::randomColors()
{
    offsetR = random(0,127);
    offsetG = random(0,127);
    offsetB = random(0,127);
    randPixel = random(0, strip.numPixels() - 1);
    randR = random(0,127) - offsetR;
    randG = random(0,127) - offsetG;
    randB = random(0,127) - offsetB;

    int i = randPixel;
    byte r = abs(0 - constrain(randR, 0, 127));
    byte g = abs(0 - constrain(randG, 0, 127));
    byte b = abs(0 - constrain(randB, 0, 127));

    strip.setPixelColor(i,r,g,b);
    digitalWrite(ADAFRUITBLE_REQ, 1);
    strip.show();
    digitalWrite(ADAFRUITBLE_REQ, 0);
}

void SimpleAnimations::microPhoneLevel() {

    //samplerate interesting to change

    unsigned long startMillis= millis();  // Start of sample window
    unsigned int peakToPeak = 0;   // peak-to-peak level

    unsigned int signalMax = 0;
    unsigned int signalMin = 1024;

    // collect data for 50 mS
    while (millis() - startMillis < sampleWindow)
    {
        sample = analogRead(0);
        if (sample < 1024)  // toss out spurious readings
        {
            if (sample > signalMax)
            {
                signalMax = sample;  // save just the max levels
            }
            else if (sample < signalMin)
            {
                signalMin = sample;  // save just the min levels
            }
        }
    }
    peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude

//    double volts = (peakToPeak * 3.3) / 1024;  // convert to volts

    Serial.println(peakToPeak);

    unsigned int numberOfLights = map(peakToPeak, 0, 1024, 0, ledCount) * 1.4;

//    Serial.println(numberOfLights);

    for (int ledPosition=0; ledPosition < strip.numPixels(); ledPosition++) {
        strip.setPixelColor(ledPosition, strip.Color(0, 0, 0));
    }
    strip.show();

    for (int ledPosition=0; ledPosition < numberOfLights; ledPosition++) {
        strip.setPixelColor(ledPosition, color);
    }
    strip.show();

}

// private

//Input a value 0 to 384 to get a color value.
//The colours are a transition r - g - b - back to r

uint32_t SimpleAnimations::Wheel(uint16_t WheelPos)
{
    byte r, g, b;
    switch(WheelPos / 128)
    {
        case 0:
            r = 127 - WheelPos % 128; // red down
            g = WheelPos % 128;       // green up
            b = 0;                    // blue off
            break;
        case 1:
            g = 127 - WheelPos % 128; // green down
            b = WheelPos % 128;       // blue up
            r = 0;                    // red off
            break;
        case 2:
            b = 127 - WheelPos % 128; // blue down
            r = WheelPos % 128;       // red up
            g = 0;                    // green off
            break;
    }
    return(strip.Color(r,g,b));
}

