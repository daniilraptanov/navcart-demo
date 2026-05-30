#pragma once

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

namespace SensorFusion {
    struct Displacement {
        float dx;
        float dy;
        bool isValid;
    };

    void setup();
    void createFusionTask();

    Displacement getDisplacement();
    void resetDisplacement();
}
