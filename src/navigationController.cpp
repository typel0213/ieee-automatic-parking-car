#include "navigationController.h"
#include "motorController.h"
#include "gyroController.h"
#include "stateMachine.h"

#include <iostream>
#include <thread>
#include <chrono>

void navigationController::realignCar(float correction) {
    motor.steer(30.0f, correction);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    if(correction < 0.0f) {
        motor.rotateLeft(correction);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        motor.forward(30.0f);
    }
    else {
        motor.rotateRight(correction);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        motor.forward(30.0f);
    }
}