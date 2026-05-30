#include "optical-flow.h"

namespace OpticalFlow {

    namespace {
        HardwareSerial SerialOF(1);
        
        int32_t accumulatedDx = 0;
        int32_t accumulatedDy = 0;
        uint8_t lastQuality = 0;

        SemaphoreHandle_t dataMutex = nullptr;

        enum ParserState {
            WAIT_HEADER,
            WAIT_LENGTH,
            READ_PAYLOAD
        };

        void sensorTask(void* parameter) {
            ParserState state = WAIT_HEADER;
            uint8_t payload[7];
            uint8_t payloadIdx = 0;

            for (;;) {
                while (SerialOF.available()) {
                    uint8_t b = SerialOF.read();

                    switch (state) {
                        case WAIT_HEADER:
                            if (b == 0xFE) {
                                state = WAIT_LENGTH;
                            }
                            break;

                        case WAIT_LENGTH:
                            if (b == 0x04) {
                                state = READ_PAYLOAD;
                                payloadIdx = 0;
                            } else if (b == 0xFE) {
                                state = WAIT_LENGTH;
                            } else {
                                state = WAIT_HEADER;
                            }
                            break;

                        case READ_PAYLOAD:
                            payload[payloadIdx++] = b;
                            
                            if (payloadIdx == 7) {
                                if (payload[6] == 0xAA) {
                                    
                                    uint8_t calculatedChecksum = payload[0] + payload[1] + payload[2] + payload[3];
                                    
                                    if (calculatedChecksum == payload[4]) {
                                        int16_t dx = (int16_t)((payload[0] << 8) | payload[1]);
                                        int16_t dy = (int16_t)((payload[2] << 8) | payload[3]);
                                        uint8_t quality = payload[5];

                                        if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
                                            accumulatedDx += dx;
                                            accumulatedDy += dy;
                                            lastQuality = quality;
                                            xSemaphoreGive(dataMutex);
                                        }
                                    } else {
                                        Serial.println("[OpticalFlow] Control sum error");
                                    }
                                }
                                state = WAIT_HEADER;
                            }
                            break;
                    }
                }
                
                vTaskDelay(pdMS_TO_TICKS(10));
            }
        }
    }

    void setup() {
        dataMutex = xSemaphoreCreateMutex();
        
        SerialOF.begin(BAUD_RATE, SERIAL_8N1, RX_PIN, -1);
        Serial.println("Optical Flow UART initialized (19200 baud).");
    }

    void createSensorTask() {
        xTaskCreate(
            sensorTask,
            "OptFlow_Task",
            4096,
            NULL,
            2,
            NULL
        );
        Serial.println("Optical Flow FreeRTOS task started.");
    }

    Motion getMotion() {
        Motion m = {0, 0, 0};
        if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
            m.dx = accumulatedDx;
            m.dy = accumulatedDy;
            m.quality = lastQuality;
            
            accumulatedDx = 0;
            accumulatedDy = 0;
            
            xSemaphoreGive(dataMutex);
        }
        return m;
    }

    void printMotion() {
        Motion m = OpticalFlow::getMotion();

        char buffer[64];
        snprintf(buffer, sizeof(buffer), "Motion -> dX: %4d | dY: %4d | Quality: %3u/255", m.dx, m.dy, m.quality);
        Serial.println(buffer);
    }
}
