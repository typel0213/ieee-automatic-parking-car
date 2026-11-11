#ifndef PARKINGCONTROLLER_H
#define PARKINGCONTROLLER_H

#include "motorController.h"

class parkingController {
    public:
        void park();
    private:
        motorController& motor;

        enum class ParkingState {
            IDLE,
            ROTATING,
            MOVING_FORWARD,
            MOVING_BACKWARD,
            PARKED,
            ERROR
        };
};

#endif