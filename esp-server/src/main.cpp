#include "./web-server/web-server.h"
#include "./monitoring/monitoring.h"

void setup() { 
    Serial.begin(115200);
    delay(1000);

    AppWebServer::setup();
}

void loop() {
    AppWebServer::dnsServer.processNextRequest();
    AppWebServer::server.handleClient();
    Monitoring::tick();
}
