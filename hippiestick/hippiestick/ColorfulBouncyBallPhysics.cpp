#include "ColorfulBouncyBallPhysics.h"

Ball balls[] = {
    Ball(stripCeiling, 25, .80, 1.0),
    Ball(stripCeiling, 30, .75, 1.4),
    Ball(stripCeiling, 15, .25, 1.6),
    Ball(stripCeiling, 50, .15, 1.9),
    Ball(stripCeiling, 15, .35, 1.11),
    Ball(stripCeiling, 9, .95, 1.6),
    Ball(stripCeiling, 25, .15, 1.22),
    Ball(stripCeiling, 11, .05, 1.8),
    Ball(stripCeiling, 150, .25, 0.4),
    Ball(stripCeiling, 40, .35, 1.1),
    Ball(stripCeiling, 25, .80, 1.0),
    Ball(stripCeiling, 1, .75, 1.4),
    Ball(stripCeiling, 25, .75, 1.2),
    Ball(stripCeiling, 50, .05, 1.9),
    Ball(stripCeiling, 15, .35, 2),
    Ball(stripCeiling, 9, .95, 1.6),
    Ball(stripCeiling, 25, .15, 1.22),
    Ball(stripCeiling, 11, .05, 1.8),
    Ball(stripCeiling, 200, .25, 2.4),
    Ball(stripCeiling, 40, .35, .8),
    Ball(stripCeiling, 1, .35, 1.0),
    Ball(stripCeiling, 20, .35, 1.1),
    Ball(stripCeiling, 30, .35, 0.3),
    Ball(stripCeiling, 40, .35, 0.2)
};

const byte ballCount = sizeof(balls) / sizeof(Ball);

ColorfulBouncyBallPhysics::ColorfulBouncyBallPhysics(void) {}

ColorfulBouncyBallPhysics::ColorfulBouncyBallPhysics(LPD8806 s) {
    strip = s;
}

void ColorfulBouncyBallPhysics::updateStrip(LPD8806 s) {
    strip = s;
}

void ColorfulBouncyBallPhysics::loop() {
    static long nextStep = 0;
    long milliSeconds = millis();

    serialAddKineticEnergy();

    if (milliSeconds >= nextStep) {
        nextStep += updateInterval;
        for (byte i = ballCount; i-- > 0;)
            balls[i].travel(timeFactor, gravity);
        renderDots();
    }
}

void ColorfulBouncyBallPhysics::serialAddKineticEnergy() {
    for (int i = 0; i < ballCount; ++i) {
        balls[i].applyForce(addedForce);
        //      Serial.print("Ball #");
        //      Serial.print(i + 1);
        //      Serial.print(": Force of ");
        //      Serial.print(addedForce);
        //      Serial.print("J changes speed from ");
        //      Serial.print(balls[i].speed, 4);
        //      Serial.print(" to ");
        //      Serial.print(balls[i].speed, 4);
        //      Serial.println("m/s");
    }
}

void ColorfulBouncyBallPhysics::renderDots() {
    for (int i = 0; i < ballCount; ++i) {
        int index = (balls[i].height + ledSpacing / 2) / ledSpacing;
        strip.setPixelColor(index, colors[random(0,24)] | strip.getPixelColor(index));
    }
    strip.show();
    for (int i = ballCount; i-- > 0;)
        strip.setPixelColor((balls[i].height + ledSpacing / 2) / ledSpacing, 0);
}