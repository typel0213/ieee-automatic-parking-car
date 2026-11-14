#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include "GPIOController.h"

class motorController {
public:
    explicit motorController(GPIOController& gpio);

    void setMotors(float left, float right);

    void forward(float speed);
    void backward(float speed);

    void rotateLeft(float speed);
    void rotateRight(float speed);
    void steer(float speed, float correction);
    
    void stop();
private:
    GPIOController& gpio;
};

#endif