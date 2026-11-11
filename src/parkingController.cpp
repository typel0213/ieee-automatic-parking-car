#include "parkingController.h"
#include "motorController.h"

void parkingController::park() {
    motor.forward(0.5f);
    motor.rotateRight(0.3f);
    motor.forward(0.5f);
}