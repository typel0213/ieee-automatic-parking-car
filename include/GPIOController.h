#ifndef GPIOCONTROLLER_H
#define GPIOCONTROLLER_H

class GPIOController {
    public:
        //Left Motor
        static constexpr int AIN1 = 0;
        static constexpr int AIN2 = 0;
        static constexpr int PWMA = 0;

        //Right Motor
        static constexpr int BIN1 = 0;
        static constexpr int BIN2 = 0;
        static constexpr int PWMB = 0;

        static constexpr int STBY = 0;

        GPIOController();
        ~GPIOController();

        void setPin(int pinNumber, bool value);
        void setPWM(int pinNumber, float dutyCycle);
        void stopAll();
        
    private:
        gpiod_line_request* request = nullptr;
    

};
#endif