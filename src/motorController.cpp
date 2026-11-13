#include "motorController.h"
#include "GPIOController.h"
#include <algorithm>

void motorController::setMotors(float left, float right) {
    // Implementation to set motor speeds
    left = std::clamp(left, -1.0f, 1.0f);
    right = std::clamp(right, -1.0f, 1.0f);
}

void motorController::forward(float speed) {
    //Implement forward movement
    setMotors(speed, speed);
}

void motorController::backward(float speed) {
    //Implement backward movement
    setMotors(-speed, -speed);
}

void motorController::rotateLeft(float speed) {
    //Implement left rotation
    setMotors(-speed, speed);
}

void motorController::rotateRight(float speed) {
    //Implement right rotation
    setMotors(speed, -speed);
}

void motorController::steer(float speed, float correction) {
    //Implement steering
    float left = speed + correction;
    float right = speed - correction;
    setMotors(left, right);
}
void motorController::stop() {
    //Implement stop
    setMotors(0.0f, 0.0f); // Stop both motors
}



