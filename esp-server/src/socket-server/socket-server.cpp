#include "socket-server.h"

namespace AppWebSocket {
    void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
        switch(type) {
            case WStype_DISCONNECTED:
                Serial.printf("[WS] Client #%u was disconnected\n", num);
                break;
                
            case WStype_CONNECTED: {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[WS] Client #%u was connected with IP: %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
                
                StaticJsonDocument<128> doc;
                doc["status"] = "connected";
                doc["message"] = "Welcome to NavCart ESP32 Server";
                
                String response;
                serializeJson(doc, response);
                webSocket.sendTXT(num, response);
                break;
            }
            
            case WStype_TEXT:
                Serial.printf("[WS] Received text from #%u: %s\n", num, payload);
                break;
                
            case WStype_ERROR:
                Serial.printf("[WS] Client Error from #%u\n", num);
                break;
                
            default:
                break;
        }
    }

    void setup() {
        webSocket.begin();
        webSocket.onEvent(webSocketEvent);
        Serial.println("WebSocket server was started on a port 81");
    }

    void broadcastRandomData() {
        StaticJsonDocument<128> doc;
        doc["action"] = "sensor_update";
        doc["value"] = random(10, 99);

        String jsonString;
        serializeJson(doc, jsonString);

        webSocket.broadcastTXT(jsonString);
    }

    void broadcastTaskCallback(void *parameter) {
        for (;;) {
            broadcastRandomData();
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }

    void createBroadcastTask() {
        xTaskCreate(
            broadcastTaskCallback,
            "WS_Broadcast_Task",
            2048,
            NULL,
            1,
            NULL
        );
    }
}
