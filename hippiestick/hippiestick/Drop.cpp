//
//  Drop.cpp
//  hippiestick
//
//  Created by Tyler Powers on 12/18/14.
//  Copyright (c) 2014 gr4yscale. All rights reserved.
//

#include "Drop.h"


Drop::Drop(float ceiling, int mass, float elasticity):
ceiling(ceiling),
mass(mass),
elasticity(elasticity) {
    direction = DOWN;
}

Drop::Drop(float ceiling, int mass, float elasticity, float height, float speed):
ceiling(ceiling),
mass(mass),
elasticity(elasticity),
height(height),
speed(speed) {
    direction = DOWN;
}

void Drop::travel(float duration, float gravity) {
    // Applies an acceleration on the Drop.
    //
    // Gravity and other downward pulling forces should
    // be given as positive numbers. Similarly, upward
    // forces should have a negative sign.
    float distance, newHeight, timeTaken, acceleration = -gravity;
    if (speed || (height < ceiling && height > 0)) {
        newHeight = height + displacementAccelerated(duration, acceleration);
        if (newHeight <= 0 || newHeight >= ceiling) {

            distance = newHeight < 0 ? -height : ceiling - height;
            if (!distance) {
                timeTaken = 0;
            } else {
                // Only calculate time required if there is distance to cover
            }
            height = ceiling;
            speed = 0;
            mass = random(1, 1000);
            applyForce(random(200, 600));

            height += displacementAccelerated(duration - timeTaken, acceleration);
        } else {
            height = newHeight;
            changeVelocity(duration, acceleration);
        }
    }
}

void Drop::applyForce(float force) {
    // Applies a certain force to the Drop in the current direction of motion.
    applyForce(force, direction);
}

void Drop::applyForce(float force, Direction dir) {
    // Applies a certain force to the Drop in the given direction.
    //
    // If the force applied is in the current direction of motion,
    // the Drop simply speeds up. In the other case, the Drop slows
    // down until it hits a dead stop. If the force is greater than
    // that, the direction is reversed and the remainder of the force
    // is applied to generate speed in the new direction.
    if (dir == direction) {
        speedFromEnergy(kineticEnergy() + force);
    } else {
        float currentEnergy = kineticEnergy();
        if (force > currentEnergy)
            reverseDirection();
        speedFromEnergy(abs(currentEnergy - force));
    }
}


void Drop::changeVelocity(float duration, float acceleration) {
    // Changes the velocity of the Drop resulting from acceleration
    float speedDiff = abs(acceleration * duration);
    if (direction == (acceleration <= 0 ? DOWN : UP)) {
        speed += speedDiff;
    } else if (speedDiff > speed) {
        speed = speedDiff - speed;
        reverseDirection();
    } else {
        speed -= speedDiff;
    }
}

void Drop::reverseDirection(void) {
    // Reverses the direction of movement of the Drop.
    direction = direction == UP ? DOWN : UP;
}

void Drop::speedFromEnergy(float energy) {
    // Resets the speed of the Drop based on its mass and given kinetic energy
    speed = sqrt(energy / mass * 2);
}

float Drop::displacementAccelerated(float duration, float acceleration) {
    // Returns the distance moved during acceleration
    return displacementConstant(duration) + 0.5 * acceleration * sq(duration);
}

float Drop::displacementConstant(float duration) {
    // Returns thh distance moved during constant velocity
    return velocity() * duration;
}

float Drop::timeForDisplacement(float distance, float acceleration) {
    // Returns the time needed to travel the given distance under acceleration
    if (!acceleration)
        return distance / speed;
    return (sqrt(sq(speed) + 2 * acceleration * distance) - speed) / acceleration;
}

float Drop::kineticEnergy(void) {
    // Returns the kinetic energy of the Drop in Joule.
    return 0.5 * mass * sq(speed);
}

float Drop::velocity(void) {
    // Returns the velocity (speed * direction) of the Drop; Positive is upward
    return speed * direction;
}

void Drop::serialReport() {
    // Reports the current height, speed and kinetic energy over serial
    Serial.print("h=");
    Serial.print(height, 7);
    Serial.print(", v=");
    Serial.print(speed, 7);
    Serial.print(", k=");
    Serial.println(kineticEnergy(), 4);
}