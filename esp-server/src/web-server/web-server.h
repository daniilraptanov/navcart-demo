#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <LittleFS.h>

namespace AppWebServer {
    void setup();
    DNSServer& getDnsServer();
    WebServer& getWebServer();

    namespace {
        static const char* ssid = "NAVCART_DEMO";
        static const byte DNS_PORT = 53;

        static DNSServer dnsServer;
        static WebServer server(80);

        void handleRoot() {};
        void handleNotFound() {};
    }
}
