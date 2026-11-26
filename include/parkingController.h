#ifndef PARKINGCONTROLLER_H
#define PARKINGCONTROLLER_H

#include <chrono>

#include "motorController.h"
#include "gyroController.h"

class parkingController {
    public:
        explicit parkingController(motorController& motor, gyroController& gyro);
        void rotateToAngle(float angle);
    private:
        motorController& motor;
        gyroController& gyro;
        float targetHeading;
        std::chrono::steady_clock::time_point rotationStartTime;

        enum class rotation {
            IDLE,
            RUNNING,
            COMPLETE,
            FAILED
        };

        rotation currentRotation = rotation::IDLE;
};

#endif