#include "ranging-sensor.h"

namespace RangingSensor {
    namespace {
        Adafruit_VL53L0X lox = Adafruit_VL53L0X();

        uint16_t lastDistance = 0;
        bool lastError = true;

        SemaphoreHandle_t dataMutex = nullptr;

        void sensorTask(void* parameter){
            VL53L0X_RangingMeasurementData_t measure;

            for(;;) {
                lox.rangingTest(&measure, false);

                uint16_t tempDistance = 0;
                bool tempError = false;

                if(measure.RangeStatus != 4){
                    tempDistance = measure.RangeMilliMeter;
                } else {
                    tempError = true;
                }

                if(xSemaphoreTake(dataMutex, portMAX_DELAY)) {
                    lastDistance = tempDistance;
                    lastError = tempError;
                    xSemaphoreGive(dataMutex);

                    vTaskDelay(pdMS_TO_TICKS(50));
                }
            }
        }
    }

    void setup() {
        dataMutex = xSemaphoreCreateMutex();
            
        Wire.begin(SDA_PIN, SCL_PIN);
            
        Serial.println("Initializing VL53L0X...");
            
        if (!lox.begin()) {
            Serial.println("[RangingSensor] ERROR: Failed to boot VL53L0X. Check wiring/power!");
        } else {
            Serial.println("[RangingSensor] VL53L0X successfully initialized.");
        }
    }

    void createSensorTask() {
        xTaskCreate(
            sensorTask,         
            "VL53L0X_Task",     
            4096,               
            NULL,               
            2,                  
            NULL                
        );
        Serial.println("[RangingSensor] FreeRTOS task started.");
    }

    Range getDistance() {
        Range r = {0, true};
        if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
            r.outDistance = lastDistance;
            r.outError = lastError;
                
            xSemaphoreGive(dataMutex);
        }

        return r;
    }

    void printDistance() {
        Range r = RangingSensor::getDistance();

        if (r.outError) {
            Serial.println("Distance -> Out of range / Error");
        } else {
            char buffer[64];
            snprintf(buffer, sizeof(buffer), "Distance -> %4u mm", r.outDistance);
            Serial.println(buffer);
        }
    }

}