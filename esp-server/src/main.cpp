#include "./web-server/web-server.h"
#include "./socket-server/socket-server.h"
#include "./monitoring/monitoring.h"
// #include "./pathfinder/pathfinder.h"

void setup() { 
    Serial.begin(115200);
    delay(1000);

    AppWebServer::setup();
    AppWebSocket::setup();

    Monitoring::setInterval(10000);

    AppWebSocket::createBroadcastTask();
    // Pathfinder::setup();
}

void loop() {
    // TODO :: it is here just for testing
    // Pathfinder::routeToDestination(2);

    AppWebServer::getDnsServer().processNextRequest();
    AppWebServer::getWebServer().handleClient();
    AppWebSocket::getWebSocket().loop();
    Monitoring::tick();
}
