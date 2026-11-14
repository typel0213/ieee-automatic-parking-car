#ifndef GPIOCONTROLLER_H
#define GPIOCONTROLLER_H

#include <gpiod.h>
#include <string>

class GPIOController {
    public:
        GPIOController(const GPIOController&) = delete;
        GPIOController& operator=(const GPIOController&) = delete;

        //Left Motor
        static constexpr int AIN1 = 27;
        static constexpr int AIN2 = 22;
        static constexpr int PWMA = 18;

        //Right Motor
        static constexpr int BIN1 = 5;
        static constexpr int BIN2 = 6;
        static constexpr int PWMB = 19;

        static constexpr int STBY = 24;

        GPIOController();
        ~GPIOController();

        void setPin(int pinNumber, bool value);
        void setPWM(int pinNumber, float dutyCycle);
        void stopAll();
        
    private:
        gpiod_line_request* request = nullptr;
        gpiod_chip* chip = nullptr;

        static constexpr int PWM_PERIOD = 1000000;
    
        std::string pwmPath = "/sys/class/pwm/pwmchip0";

        void initializeGPIO();
        void initializePWM();

        void writePWMFile(const std::string& path, int value);
};
#endif