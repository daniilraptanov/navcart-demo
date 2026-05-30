#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

namespace OpticalFlow {
    const int RX_PIN = 20;
    const int BAUD_RATE = 19200;

    struct Motion {
        int32_t dx;
        int32_t dy;
        uint8_t quality;
    };

    void setup();
    void createSensorTask();
    
    Motion getMotion();
    void printMotion();
}
