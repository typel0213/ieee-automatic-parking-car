#ifndef NAVIGATIONCONTROLLER_H
#define NAVIGATIONCONTROLLER_H

#include "motorController.h"
#include "gyroController.h"

class navigationController {
    public:
        explicit navigationController(motorController& motor, gyroController& gyro);
        void realignCar(int pixel, float desiredHead);
    private:
        motorController& motor;
        gyroController& gyro;
        float correction = 0.0f;
};

#endif