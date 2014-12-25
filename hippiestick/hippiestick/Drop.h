//
//  Drop.h
//  hippiestick
//
//  Created by Tyler Powers on 12/18/14.
//  Copyright (c) 2014 gr4yscale. All rights reserved.
//

#ifndef __hippiestick__Drop__
#define __hippiestick__Drop__

#include <stdio.h>

#endif /* defined(__hippiestick__Drop__) */

#include "Arduino.h"

class Drop {
public:
    enum Direction {
        DOWN = -1,
        UP = 1
    };
    Drop(float ceiling, int mass, float elasticity),
    Drop(float ceiling, int mass, float elasticity, float height, float speed = 0);
    const float elasticity;
    float
    height,
    speed,
    kineticEnergy(void),
    velocity(void);
    void
    applyForce(float force),
    applyForce(float force, Direction dir),
    serialReport(),
    travel(float duration, float acceleration = 0);

private:
    int mass;
    const float ceiling;
    Direction direction;
    float
    displacementAccelerated(float duration, float acceleration),
    displacementConstant(float duration),
    timeForDisplacement(float distance, float acceleration);
    void
    bounce(void),
    changeVelocity(float acceleration, float duration),
    reverseDirection(void),
    speedFromEnergy(float energy);
};

