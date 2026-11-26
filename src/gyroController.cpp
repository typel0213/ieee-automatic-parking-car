#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <thread>
#include <chrono>

#include "gyroController.h"

gyroController::gyroController() {
    //Initialize GPIO pins 2 and 3
    file = open("/dev/i2c-1", O_RDWR);
    if(file < 0) { //Error check
        throw std::runtime_error("Error opening /dev/i2c-1");
    }
    if(ioctl(file, I2C_SLAVE, 0x68) == -1) {
        close(file);
        throw std::runtime_error("Error connecting to address 0x68");
    }
    
    //Turns on MPU
    uint8_t wake[2] = {0x6B, 0x00};
    if(write(file, wake, 2) != 2) { //Error check
        close(file);
        throw std::runtime_error("Error waking MPU-6050");
    }

    //Set filter of 42 Hz to Digital Low-Pass Filter
    uint8_t dlpf[2] = {0x1A, 0x03};
    if(write(file, dlpf, 2) != 2) {
        close(file);
        throw std::runtime_error("Error opening /dev/i2c-1");
    }

    uint8_t gyroConfig[2] = {0x1B, 0x00};
    if(write(file, gyroConfig, 2) != 2) {
        close(file);
        throw std::runtime_error("Error setting gyro range");
    }
}

gyroController::~gyroController() {
    if(file >= 0) {
        close(file);
    }
}

float gyroController::readGyro() {
    //Points to first gyroscope data register
    uint8_t reg = 0x43;
    if(write(file, &reg, 1) != 1) {
        throw std::runtime_error("Failed to request gyro registor");
    }
    //Reads 6 consecutive bytes (0x43 to 0x48)
    uint8_t data[6];
    if(read(file, data, 6) != 6) {
        throw std::runtime_error("Failed to read all registers");
    }

    //float x = (int16_t)((data[0]) << 8 | data[1]) / GYRO_SENS;
    //float y = (int16_t)((data[2]) << 8 | data[3]) / GYRO_SENS;
    float z = (int16_t)((data[4]) << 8 | data[5]) / GYRO_SENS;

    return z;
}

void gyroController::calibrateGyro() {
    hasPreviousTime = false;
    isCalibrated = false;
    float total = 0.0f;
    int n = 200;
    for(int i = 0; i < n; i++) {
        total += readGyro();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    gyrobiasZ = total/n;
    isCalibrated = true;
}

void gyroController::updateHeading() {
    if(isCalibrated == false) {
        return;
    }
    else if (!hasPreviousTime){
        previousTime = std::chrono::steady_clock::now();
        hasPreviousTime = true;
        return;
    }
    if(hasPreviousTime) {
        std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
        float timeDifference = std::chrono::duration<float>(currentTime - previousTime).count();
        float newAngle = heading + (readGyro() - gyrobiasZ) * timeDifference;
        previousTime = currentTime;
        heading = newAngle;
    }
}

float gyroController::getHeading() {
    return heading;
}