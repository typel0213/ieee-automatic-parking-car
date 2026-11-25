#ifndef NAVIGATIONCONTROLLER_H
#define NAVIGATIONCONTROLLER_H

#include "motorController.h"

class navigationController {
    public:
        void realignCar(float correction);
    private:
        motorController& motor;

        enum class centerState {
            CENTERED,
            OFFCENTERED,
            REALIGNING
        }
}

#endif