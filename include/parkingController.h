#ifndef PARKINGCONTROLLER_H
#define PARKINGCONTROLLER_H

#include "motorController.h"

class parkingController {
    public:
        void parkRight();
        void parkLeft();
    private:
        motorController& motor;
};

#endif