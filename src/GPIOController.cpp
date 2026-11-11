#include "GPIOController.h"

GPIOController::GPIOController() {

}

GPIOController::~GPIOController() {

}

void GPIOController::setPin(int pin, bool value) {

}

void GPIOController::setPWM(int pin, float dutyCycle) {
    
}

void GPIOController::stopAll() {
    setPin(AIN1, false);
    setPin(AIN2, false);
    setPin(PWMA, false);
    setPin(BIN1, false);
    setPin(BIN2, false);
    setPin(PWMB, false);
    setPin(STBY, false);
}