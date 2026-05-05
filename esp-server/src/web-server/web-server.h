#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <LittleFS.h>

namespace AppWebServer {
    DNSServer dnsServer;
    WebServer server;
    void setup();
}
