    #include "parkingController.h"
#include "motorController.h"
#include "gyroController.h"
#include "stateMachine.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

parkingController::parkingController(motorController& motorControl, gyroController& gyroControl) 
: motor(motorControl) : gyro(gyroControl){
    
}

parkingController::rotation parkingController::rotateToAngle(float angle) {
    float currentHeading = gyro.getHeading();
    if(currentRotation == rotation::FAILED) {
        motor.stop();
        return currentRotation;
    }
    if(currentRotation == rotation::COMPLETE) {
        motor.stop();
        return currentRotation;
    }

    if(currentRotation == rotation::IDLE) {
        targetHeading = currentHeading + angle;
        rotationStartTime = std::chrono::steady_clock::now();
        currentRotation = rotation::RUNNING;
    }

    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();

    if(currentRotation == rotation::RUNNING && ((currentTime - rotationStartTime) > std::chrono::milliseconds(5000))) {
        motor.stop();
        currentRotation = rotation::FAILED;
        return currentRotation;
    }
    
    if(currentRotation == rotation::RUNNING && (std::abs(targetHeading - currentHeading) < 3.0f)) {
        currentRotation = rotation::COMPLETE;
        motor.stop();
        return currentRotation;
    }

    if(targetHeading < currentHeading && currentRotation == rotation::RUNNING) {
        motor.rotateRight(30.0f);
    }
    else if(targetHeading > currentHeading && currentRotation == rotation::RUNNING) {
        motor.rotateLeft(30.0f);
    }
    return currentRotation;
}