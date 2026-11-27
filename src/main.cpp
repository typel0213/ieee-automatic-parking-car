#include "GPIOController.h"
#include "gyroController.h"
#include "motorController.h"
#include "parkingController.h"
#include "navigationController.h"
#include "stateMachine.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <exception>
#include <array>
#include <cstddef>

int main() {
    try {
        GPIOController gpio;
        motorController motor(gpio);
        gyroController gyro;
        //socketController socket;
        stateMachine state;
        parkingController parking(motor, gyro);
        navigationController navigation(motor, gyro);

        gyro.calibrateGyro();
        
        float originalHead = gyro.getHeading();
        float parkingHead = 0.0f;

        while(state.getState() != state::states::COMPLETE && state.getState() != state::states::ERROR) {
            StateCondition conditions;
            gyro.updateHeading();
            //Get cameraInfo
            //int pixel = pixeloffset from socket
            //int orientation = -1 or 1 from socket
            //Check for errors
            state.updateState(conditions);

            switch(state.getState()) {
                case state::states::IDLE:
                    motor.stop();
                    break;
                case state::states::TRACK_LANE:
                    navigation.realignCar(pixel, originalHead);
                    break;
                case state::states::TRACK_PARKING:
                    navigation.realignCar(pixel, originalHead);
                    break;
                case state::states::ROTATING: {
                    parkingController::rotation currentParking = parking.rotateToAngle(90 * orientation);
                    if(currentParking == parkingController::rotation::COMPLETE) {
                        conditions.rotationComplete = true;
                        state.updateState(conditions);
                        parkingHead = gyro.getHeading();
                    }
                    else if (currentParking == parkingController::rotation::FAILED){
                        conditions.timedOut = true;
                        state.updateState(conditions);
                    }
                    break;
                }
                case state::states::PARKING_GUIDE:
                    navigation.realignCar(pixel, parkingHead);
                    break;
                case state::states::COMPLETE:
                    motor.stop();
                    break;
                case state::states::ERROR:
                    motor.stop();
                    break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        motor.stop();
    }
    catch(const std::exception& e) {
        
    }
    return 0;
}