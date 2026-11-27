#ifndef PARKINGCONTROLLER_H
#define PARKINGCONTROLLER_H

#include <chrono>

#include "motorController.h"
#include "gyroController.h"

class parkingController {
    public:
        explicit parkingController(motorController& motor, gyroController& gyro);
        rotation rotateToAngle(float angle);

        enum class rotation {
            IDLE,
            RUNNING,
            COMPLETE,
            FAILED
        };
    private:
        motorController& motor;
        gyroController& gyro;
        float targetHeading;
        std::chrono::steady_clock::time_point rotationStartTime;

        rotation currentRotation = rotation::IDLE;
};

#endif