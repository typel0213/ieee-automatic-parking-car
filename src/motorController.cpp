#include "motorController.h"
#include "GPIOController.h"

void motorController::setMotors(float left, float right) {
    // Implementation to set motor speeds
    left = std::clamp(left, -100.0f, 100.0f);
    right = std::clamp(right, -100.0f, 100.0f);
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



