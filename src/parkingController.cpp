#include "parkingController.h"
#include "motorController.h"
#include "gyroController.h"
#include "stateMachine.h"

#include <iostream>
#include <thread>
#include <chrono>

parkingController::parkingController(motorController& motorControl, gyroController& gyroControl) : motor(motorControl) : gyro(gyroControl){
    
}

void parkingController::parking() {
    motor.forward(30.0f);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    rotateToAngle(angle);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    motor.stop();
}

bool parkingController::rotateToAngle(float angle) {
    float currentHeading = gyro.getHeading();
    float newHeading = currentHeading + angle;
    while(gyro.getHeading() != newHeading) {
        if(newHeading < currentHeading)
            motor.rotateRight(50.0f);
        else if(newHeading > currentHeading) {
            motor.rotateLeft(50.0f);
        }
    }
    if(gyro.getHeading() == newHeading) {
        return true;
    }
    return false;
}