#pragma once

#include <Arduino.h>

// TODO :: replace all constants to /data dir
// TODO :: use RTOS features for routes calculating

namespace Pathfinder {
    
    const int MAP_WIDTH = 10;
    const int MAP_HEIGHT = 10;

    const int CURRENT_X = 0;
    const int CURRENT_Y = 0;

    const uint8_t OBSTACLES_MAP[MAP_WIDTH * MAP_HEIGHT] = {
        0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 1, 1, 1, 0, 1, 0, 1, 1, 0,
        0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
        0, 1, 0, 1, 1, 1, 1, 1, 0, 1,
        0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
        0, 1, 1, 1, 1, 1, 0, 1, 1, 1,
        0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
        1, 1, 1, 1, 0, 1, 1, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 0 
    };

    const uint8_t DESTINATIONS_MAP[MAP_WIDTH * MAP_HEIGHT] = {
        0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 3, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 3, 0, 0, 0,  
        0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 0, 0, 0, 0, 0, 0, 0,  
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 2, 2, 2, 2, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0 
    };

    void setup();
    void routeToDestination(uint8_t destId);
}
