#include "motorController.h"
#include "GPIOController.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

motorController::motorController(GPIOController& controller) : gpio(controller) {
    
}

void motorController::setMotors(float left, float right) {
    if(!std::isfinite(left) || !std::isfinite(right)) {
        throw std::invalid_argument("Invalid motor speed");
    }

    // Implementation to set motor speeds
    left = std::clamp(left, -100.0f, 100.0f);
    right = std::clamp(right, -100.0f, 100.0f);

    gpio.setPin(GPIOController::STBY, false);

    gpio.setPWM(GPIOController::PWMA, 0.0f);
    gpio.setPWM(GPIOController::PWMB, 0.0f);

    gpio.setPin(GPIOController::AIN1, left > 0);
    gpio.setPin(GPIOController::AIN2, left < 0);

    gpio.setPin(GPIOController::BIN1, right > 0);
    gpio.setPin(GPIOController::BIN2, right < 0);

    gpio.setPWM(GPIOController::PWMA, std::abs(left));
    gpio.setPWM(GPIOController::PWMB, std::abs(right));

    if(left != 0.0f || right != 0.0f) {
        gpio.setPin(GPIOController::STBY, true);
    }
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



