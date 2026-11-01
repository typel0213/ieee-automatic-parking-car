#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

class motorController {
public:
    void setMotors(float left, float right);
    void forward(float speed);
    void backward(float speed);
    void rotateLeft(float speed);
    void rotateRight(float speed);

    void stop();
};

#endif