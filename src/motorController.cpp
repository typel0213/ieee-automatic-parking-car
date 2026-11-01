#include "motorController.h"
#include "GPIOController.h"

void motorController::setMotors(float left, float right) {
    // Implementation to set motor speeds
    
}

void motorController::forward(int speed) {
    //Implement forward movement
    setMotors(speed, speed);
}

void motorController::backward(int speed) {
    //Implement backward movement
    setMotors(speed, speed);
}

void motorController::rotateLeft(int speed) {
    //Implement left rotation
    setMotors(-speed, speed);
}

void motorController::rotateRight(int speed) {
    //Implement right rotation
    setMotors(speed, -speed);
}

void motorController::stop() {
    //Implement stop
    setMotors(0.0f, 0.0f); // Stop both motors
}



