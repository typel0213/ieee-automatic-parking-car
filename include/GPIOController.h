#ifndef GPIOCONTROLLER_H
#define GPIOCONTROLLER_H

class GPIOController {
    public:
        void setPin(int pinNumber);
    private:
        int pin;
};