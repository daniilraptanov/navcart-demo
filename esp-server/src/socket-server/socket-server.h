#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WebSocketsServer.h>

namespace AppWebSocket {
    void setup();
    void createBroadcastTask();
    
    WebSocketsServer& getWebSocket();

    namespace {
        WebSocketsServer webSocket = WebSocketsServer(81);
    }
}
