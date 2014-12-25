#include "ColorfulBouncyBallPhysics.h"

Drop balls[] = {
    Drop(stripCeiling, 295, .80, 1.0),
    Drop(stripCeiling, 90, .75, 1.4),
    Drop(stripCeiling, 15, .25, 1.6),
    Drop(stripCeiling, 80, .15, 1.9),
    Drop(stripCeiling, 150, .35, 1.11),
    Drop(stripCeiling, 9, .95, 1.6),
    Drop(stripCeiling, 69, .15, 1.22),
    Drop(stripCeiling, 30, .90, 1.22),
    Drop(stripCeiling, 200, .50, 1.22),
    Drop(stripCeiling, 20, .30, 1.22),
    Drop(stripCeiling, 99, .50, 1.22),
    Drop(stripCeiling, 40, .20, 1.22),
    Drop(stripCeiling, 690, .54, 1.22),
    Drop(stripCeiling, 10, .51, 1.22)

};

const byte ballCount = sizeof(balls) / sizeof(Drop);

ColorfulBouncyBallPhysics::ColorfulBouncyBallPhysics(void) {}

ColorfulBouncyBallPhysics::ColorfulBouncyBallPhysics(LPD8806 s) {
    strip = s;

    for (int i = 0; i < ballCount; ++i) {
        balls[i].applyForce(random(200,2000));
    }
}

void ColorfulBouncyBallPhysics::setStrip(LPD8806 s) {
    strip = s;
}

void ColorfulBouncyBallPhysics::loop() {
    static long nextStep = 0;
    long milliSeconds = millis();

    gravity = 80.5;

    if (milliSeconds >= nextStep) {
        nextStep += updateInterval;
        for (byte i = ballCount; i-- > 0;)
            balls[i].travel(timeFactor, gravity);
        renderDots();
    }
}


void ColorfulBouncyBallPhysics::renderDots() {
    for (int i = 0; i < ballCount; ++i) {
        int index = balls[i].height;
        strip.setPixelColor(index,0,0,255);
    }
    strip.show();
    for (int i = ballCount; i-- > 0;)
        strip.setPixelColor(balls[i].height, 0);
}