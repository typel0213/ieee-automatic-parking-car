//#include "parkingController.h"
#include "GPIOController.h"
#include "motorController.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <exception>

int main() {
    try {
        GPIOController gpio;
        motorController motor(gpio);

        std::cout << "Testing left motor\n";

        motor.setMotors(30.0f, 0.0f);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        motor.stop();

        std::cout << "Testing right motor\n";

        motor.setMotors(0.0f,30.0f);

        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        motor.stop();

        std::cout << "Motor test completed!\n";
    }
    catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}