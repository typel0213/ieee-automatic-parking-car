#include "GPIOController.h"
#include <gpiod.h>
#include <stdexcept>

GPIOController::GPIOController() {

}

GPIOController::~GPIOController() {
    gpiod_line_request_release();
    gpiod_chip_close();
    gpiod_line_settings_free();
    gpiod_line_config_free();
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
}