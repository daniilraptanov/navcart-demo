#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

namespace OpticalFlow {
    const int RX_PIN = 20;
    const int BAUD_RATE = 19200;

    void setup();
    void createSensorTask();
    
    void getMotion(int32_t& outDx, int32_t& outDy, uint8_t& outQuality);
    void printMotion();
}
