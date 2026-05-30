#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

namespace RangingSensor{
    const int SDA_PIN = 8;
    const int SCL_PIN = 9;

    void setup();
    void createSensorTask();
    
    struct Range {
        uint16_t outDistance;
        bool outError;
    };

    Range getDistance();
    void printDistance();
}
