#include "parkingController.h"
#include "motorController.h"

#include <iostream>
#include <thread>
#include <chrono>

void parkingController::parkRight() {
    motor.forward(30.0f);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    motor.rotateRight(30.0f);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    motor.forward(30.0f);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    motor.stop();

}

void parkingController::parkLeft() {
    motor.forward(30.0f);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    motor.rotateLeft(30.0f);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    motor.forward(30.0f);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    motor.stop();
}