#include "monitoring.h"

namespace Monitoring {
    static unsigned long lastMemCheck = 0;
    static unsigned long checkInterval = 5000;

    void setInterval(unsigned long interval_ms) {
        checkInterval = interval_ms;
    }

    void tick() {
        unsigned long currentMillis = millis();

        if (currentMillis - lastMemCheck >= checkInterval) {
            lastMemCheck = currentMillis;

            uint32_t totalRAM = ESP.getHeapSize();
            uint32_t freeRAM = ESP.getFreeHeap();
            uint32_t usedRAM = totalRAM - freeRAM;            
            uint32_t minFreeRAM = ESP.getMinFreeHeap();

            Serial.println("=== RAM Monitoring ===");
            Serial.printf("Total RAM: %u bytes\n", totalRAM);
            Serial.printf("Used RAM:  %u bytes\n", usedRAM);
            Serial.printf("Free RAM:  %u bytes\n", freeRAM);
            Serial.printf("Min Free:  %u bytes\n", minFreeRAM);
            Serial.println("======================");
        }
    }

}
