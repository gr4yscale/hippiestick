#include "ColorfulBouncyBallPhysics.h"

Ball balls[] = {
    Ball(stripCeiling, 25, .80, 1.0),
    Ball(stripCeiling, 30, .75, 1.4),
    Ball(stripCeiling, 15, .25, 1.6),
    Ball(stripCeiling, 50, .15, 1.9),
    Ball(stripCeiling, 15, .35, 1.11),
    Ball(stripCeiling, 9, .95, 1.6),
    Ball(stripCeiling, 25, .15, 1.22)
};

const byte ballCount = sizeof(balls) / sizeof(Ball);

ColorfulBouncyBallPhysics::ColorfulBouncyBallPhysics(void) {}

ColorfulBouncyBallPhysics::ColorfulBouncyBallPhysics(LPD8806 s) {
    strip = s;
}

void ColorfulBouncyBallPhysics::setStrip(LPD8806 s) {
    strip = s;
}

void ColorfulBouncyBallPhysics::loop() {
    static long nextStep = 0;
    long milliSeconds = millis();

    double factor = map(analogRead(1), 0, 1024.0, 0, 100);
    gravity = factor;

    addedForce = 1.0;

    serialAddKineticEnergy();


    if (milliSeconds >= nextStep) {
        nextStep += updateInterval;
        for (byte i = ballCount; i-- > 0;)
            balls[i].travel(timeFactor, gravity);
        renderDots();
    }

    Serial.println(factor);
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
        strip.setPixelColor(index, colors[random(0,ballCount-1)] | strip.getPixelColor(index));
    }
    strip.show();
    for (int i = ballCount; i-- > 0;)
        strip.setPixelColor((balls[i].height + ledSpacing / 2) / ledSpacing, 0);
}