#include "GPIOController.h"
#include <gpiod.h>
#include <stdexcept>
#include <fstream>
#include <string>
#include <algorithm>

void GPIOController::initializeGPIO() {
    unsigned int pins[] = {AIN1, AIN2, BIN1, BIN2, STBY};

    chip = gpiod_chip_open("/dev/gpiochip0");

    if(!chip) {
        throw std::runtime_error("Chip failed to open\n");
    }

    gpiod_line_settings* settings = gpiod_line_settings_new();
    gpiod_line_config* config = gpiod_line_config_new();

    if(!settings || !config) {
        gpiod_line_settings_free(settings);
        gpiod_line_config_free(config);

        throw std::runtime_error("GPIO configuration failed");
    }

    if(gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT) < 0) {
        gpiod_line_settings_free(settings);
        gpiod_line_config_free(config);

        throw std::runtime_error("Failed to set GPIO direction");
    }

    if(gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE) < 0) {
        gpiod_line_settings_free(settings);
        gpiod_line_config_free(config);

        throw std::runtime_error("Failure to set GPIO output value");
    }
    
    
    int result = gpiod_line_config_add_line_settings(config, pins, 5, settings);
    

    if(result < 0) {
        gpiod_line_settings_free(settings);
        gpiod_line_config_free(config);

        throw std::runtime_error("Failed to configure pins");
    }
    
    
    request = gpiod_chip_request_lines(chip, nullptr, config);

    gpiod_line_settings_free(settings);
    gpiod_line_config_free(config);

    if(!request) {
        throw std::runtime_error("Failed to request GPIO pins");
    }

}

void GPIOController::writePWMFile(const std::string& path, int value) {
    std::ofstream file(path);

    if(!file.is_open()) {
        throw std::runtime_error("Failed to open PWM file " + path);
    }

    file << value;
    file.flush();

    if(!file) {
        throw std::runtime_error("Failed to write PWM file" + path);
    }
}

void GPIOController::initializePWM() {
    std::string pwm0 = pwmPath + "/pwm0";
    std::string pwm1 = pwmPath + "/pwm1";

    writePWMFile(pwm0 + "/enable", 0);
    writePWMFile(pwm1 + "/enable", 0);

    writePWMFile(pwm0 + "/duty_cycle", 0);
    writePWMFile(pwm1 + "/duty_cycle", 0);

    writePWMFile(pwm0 + "/period", PWM_PERIOD);
    writePWMFile(pwm1 + "/period", PWM_PERIOD);

    writePWMFile(pwm0 + "/enable", 1);
    writePWMFile(pwm1 + "/enable", 1);
}

GPIOController::GPIOController() {
    try {
        initializeGPIO();
        initializePWM();
    }
    catch (...) {
        // Disable motor driver first
        if (request) {
            gpiod_line_request_set_value(
                request,
                STBY,
                GPIOD_LINE_VALUE_INACTIVE
            );
        }

        // Attempt to disable both PWM channels
        try {
            writePWMFile(pwmPath + "/pwm0/enable", 0);
        }
        catch (...) {}

        try {
            writePWMFile(pwmPath + "/pwm1/enable", 0);
        }
        catch (...) {}

        if (request) {
            gpiod_line_request_release(request);
        }

        if (chip) {
            gpiod_chip_close(chip);
        }

        throw;
    }
}

GPIOController::~GPIOController() {
    if(request) {
        gpiod_line_request_set_value(request, STBY, GPIOD_LINE_VALUE_INACTIVE);
    }

    try {
        writePWMFile(pwmPath + "/pwm0/enable", 0);
    }
    catch (...) {
        
    }

    try {
        writePWMFile(pwmPath + "/pwm1/enable", 0);
    }
    catch (...) {

    }

    if(request) {
        gpiod_line_request_release(request);
    }

    if(chip) {
        gpiod_chip_close(chip);
    }
}

void GPIOController::setPin(int pin, bool value) {
    gpiod_line_value state = value ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE;

    int result = gpiod_line_request_set_value(request, pin, state);

    if(result < 0) {
        throw std::runtime_error("Failed to set value");
    }
}

void GPIOController::setPWM(int pin, float dutyCycle) {
    dutyCycle = std::clamp(dutyCycle, 0.0f, 100.0f);

    std::string pwmChannel;

    if(pin == PWMA) {
        pwmChannel = pwmPath + "/pwm0";
    }
    else if (pin == PWMB) {
        pwmChannel = pwmPath + "/pwm1";
    }
    else {
        throw std::invalid_argument("Invalid PWM pin");
    }

    int dutyNS = static_cast<int>(PWM_PERIOD * (dutyCycle / 100.0f));

    writePWMFile(pwmChannel + "/duty_cycle", dutyNS);
}

void GPIOController::stopAll() {
    setPin(STBY, false);
    
    setPWM(PWMA, 0.0f);
    setPWM(PWMB, 0.0f);

    setPin(AIN1, false);
    setPin(AIN2, false);
    setPin(BIN1, false);
    setPin(BIN2, false);
}