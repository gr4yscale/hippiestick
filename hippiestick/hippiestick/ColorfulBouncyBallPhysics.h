// Core library for code-sense - IDE-based
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad specific
#include "Energia.h"
#elif defined(MICRODUINO) // Microduino specific
#include "Arduino.h"
#elif defined(TEENSYDUINO) // Teensy specific
#include "Arduino.h"
#elif defined(ARDUINO) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#else // error
#error Platform not defined
#endif // end IDE

#ifndef ColorfulBouncyBallPhysics_h
#define ColorfulBouncyBallPhysics_h

#include "LPD8806.h"
#include "ball.h"
#include "SharedLibrary.h"

const int
    ledPerMeter = 52,
    updateInterval = 1;

const float
    ledSpacing = 0.75 / ledPerMeter,
    stripCeiling = (ledCount - 1) * ledSpacing,
    timeFactor = updateInterval / 1000.0;

class ColorfulBouncyBallPhysics {
    unsigned long addedForce = 2;
    float gravity = 6; // Earth gravity in m/s^2 = 1.622, Lunar gravity in m/s^2

public:
    ColorfulBouncyBallPhysics(LPD8806 s);

    void loop();
    void serialAddKineticEnergy();
    void renderDots();


private:
    LPD8806 strip;

    const long colors[23] = {
        strip.Color(127, 0, 100),
        strip.Color(0, 127, 50),
        strip.Color(10, 0, 127),
        strip.Color(100, 120, 3),
        strip.Color(40, 127, 40),
        strip.Color(10, 100, 0),
        strip.Color(90, 2, 27),
        strip.Color(127, 127, 127),
        strip.Color(60, 89, 7),
        strip.Color(2, 70, 70),
        strip.Color(127, 0, 100),
        strip.Color(0, 127, 50),
        strip.Color(10, 0, 127),
        strip.Color(100, 120, 3),
        strip.Color(40, 127, 40),
        strip.Color(10, 100, 0),
        strip.Color(90, 2, 27),
        strip.Color(127, 127, 127),
        strip.Color(60, 89, 7),
        strip.Color(2, 70, 70),
        strip.Color(12, 127, 127),
        strip.Color(120, 120, 7),
        strip.Color(2, 120, 30)
    };
};

#endif
