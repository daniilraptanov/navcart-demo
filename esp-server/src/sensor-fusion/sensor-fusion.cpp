#include "sensor-fusion.h"

namespace SensorFusion {
    static SemaphoreHandle_t dataMutex = NULL;

    static float accumulatedDxCm = 0.0f;
    static float accumulatedDyCm = 0.0f;

    const uint8_t MIN_OPTICAL_QUALITY = 50;
    const float MIN_VALID_DISTANCE_CM = 2.0f;
    const float MAX_VALID_DISTANCE_CM = 200.0f;
    const float OPTICAL_SCALE_FACTOR = 0.01f;

    static Displacement calculateDisplacementStep(int32_t optDxPixels, int32_t optDyPixels, uint8_t optQuality, float distanceCm) {
        if (
            (optQuality < MIN_OPTICAL_QUALITY) || 
            (distanceCm < MIN_VALID_DISTANCE_CM || distanceCm > MAX_VALID_DISTANCE_CM)
        ) {
            return {0.0f, 0.0f, false};
        }

        float dx = (float)optDxPixels * distanceCm * OPTICAL_SCALE_FACTOR;
        float dy = (float)optDyPixels * distanceCm * OPTICAL_SCALE_FACTOR;

        return {dx, dy, true};
    }

    void setup() {
        dataMutex = xSemaphoreCreateMutex();
        resetDisplacement();
    }

    static void fusionTask(void* parameter) {
        const TickType_t xDelay = 50 / portTICK_PERIOD_MS; 

        for (;;) {
            // TODO: Отримати дані з сенсорів
            // OpticalFlow::Motion optMotion = OpticalFlow::getMotion();
            // float distance = RangeSensor::getDistanceCm();
            // ... і потім передати optMotion.dx, optMotion.dy, optMotion.quality ...
            int32_t optDx = 0, optDy = 0;
            uint8_t optQuality = 0;
            float distance = 0.0f;
            // ----------------------------------------------------------------------

            Displacement step = calculateDisplacementStep(optDx, optDy, optQuality, distance);

            if (step.isValid) {
                if (dataMutex != NULL && xSemaphoreTake(dataMutex, portMAX_DELAY) == pdTRUE) {
                    accumulatedDxCm += step.dx;
                    accumulatedDyCm += step.dy;
                    xSemaphoreGive(dataMutex);
                }
            }

            vTaskDelay(xDelay);
        }
    }

    void createFusionTask() {
        xTaskCreate(
            fusionTask,
            "SensorFusionTask",
            4096,
            NULL,
            1,
            NULL
        );
    }

    Displacement getDisplacement() {
        Displacement result = {0.0f, 0.0f, true};
        
        if (dataMutex != NULL && xSemaphoreTake(dataMutex, portMAX_DELAY) == pdTRUE) {
            result.dx = accumulatedDxCm;
            result.dy = accumulatedDyCm;
            xSemaphoreGive(dataMutex);
        }
        
        return result;
    }

    void resetDisplacement() {
        if (dataMutex != NULL && xSemaphoreTake(dataMutex, portMAX_DELAY) == pdTRUE) {
            accumulatedDxCm = 0.0f;
            accumulatedDyCm = 0.0f;
            xSemaphoreGive(dataMutex);
        }
    }
}
