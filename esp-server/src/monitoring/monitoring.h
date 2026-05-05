#pragma once

#include <Arduino.h>

namespace Monitoring {
    void setInterval(unsigned long interval_ms);
    void tick();
}
