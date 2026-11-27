#include "navigationController.h"
#include "motorController.h"
#include "gyroController.h"
#include "stateMachine.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <algorithm>

navigationController::navigationController(motorController& motorControl, gyroController& gyroControl) 
: motor(motorControl) : gyro(gyroControl){

}

void navigationController::realignCar(int pixel, float desiredHeading) {
    float currentHeading = gyro.getHeading();

    if(abs(pixel) < 10 && (abs(desiredHeading - currentHeading) < 3.0f)) {
        motor.forward(30.0f);
        return;
    }

    correction = pixel * 0.2 + (desiredHeading - currentHeading) * 0.2;
    if(abs(correction) > 15) {
        correction = std::clamp(correction,-15.0f,15.0f);
    }
    motor.steer(30.0f, correction);
    
}