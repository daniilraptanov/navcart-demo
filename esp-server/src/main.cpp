#include "./web-server/web-server.h"
#include "./socket-server/socket-server.h"
#include "./monitoring/monitoring.h"
#include "./optical-flow/optical-flow.h"
#include "./ranging-sensor/ranging-sensor.h"
// #include "./pathfinder/pathfinder.h"

void setup() { 
    Serial.begin(115200);
    delay(1000);

    AppWebServer::setup();
    AppWebSocket::setup();
    RangingSensor::setup();

    // Pathfinder::setup();

    Monitoring::setInterval(10000);

    AppWebSocket::createBroadcastTask();
    RangingSensor::createSensorTask();
}

void loop() {
    // TODO :: it is here just for testing
    // Pathfinder::routeToDestination(2);

    AppWebServer::getDnsServer().processNextRequest();
    AppWebServer::getWebServer().handleClient();
    AppWebSocket::getWebSocket().loop();
    Monitoring::tick();

    // TODO :: it is here just for testing
    RangingSensor::printDistance();
}

