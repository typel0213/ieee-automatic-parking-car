#ifndef PARKINGCONTROLLER_H
#define PARKINGCONTROLLER_H

#include "motorController.h"
#include "gyroController.h"

class parkingController {
    public:
        explicit parkingController(motorController& motor, gyroController& gyro);
        bool rotateToAngle(float angle);
        void parking();
    private:
        motorController& motor;
        gyroController& gyro;
};

#endif