#ifndef GYROCONTROLLER_H
#define GYROCONTROLLER_H

#include <chrono>

class gyroController {
    public:
        /*struct AngVel{
            float gyx;
            float gyy;
            float gyz;;
        };
        */

        gyroController();
        ~gyroController();
        void calibrateGyro();
        void updateHeading();
        float getHeading();
        
    private:
        int file = -1;
        //Convert to angle
        static const float GYRO_SENS = 131.0f;

        float gyrobiasZ = 0.0f; //Initalize bias at Z axis
        float heading = 0.0f; //Current angle
        std::chrono::steady_clock::time_point previousTime = {}; //Initialize previous time stamp
        bool isCalibrated = false;
        bool hasPreviousTime = false;
        
};
#endif