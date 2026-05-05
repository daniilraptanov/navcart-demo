#include <Arduino.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <LittleFS.h>

#include "./monitoring/monitoring.h"

const char* ssid = "NAVCART_DEMO";
const byte DNS_PORT = 53;

DNSServer dnsServer;
WebServer server(80);

String getContentType(String filename) {
    if (filename.endsWith(".html")) return "text/html";
    else if (filename.endsWith(".css")) return "text/css";
    else if (filename.endsWith(".js")) return "application/javascript";
    else if (filename.endsWith(".ico")) return "image/x-icon";
    else if (filename.endsWith(".gz")) return "application/x-gzip";
    return "text/plain";
}

void handleRoot() {
    if (LittleFS.exists("/index.html")) {
        File file = LittleFS.open("/index.html", "r");
        server.streamFile(file, "text/html");
        file.close();
    } else {
        server.send(404, "text/plain", "File index.html not found in LittleFS");
    }
}

void handleNotFound() {
    String path = server.uri();
    if (LittleFS.exists(path)) {
        File file = LittleFS.open(path, "r");
        server.streamFile(file, getContentType(path));
        file.close();
        return;
    }
    server.sendHeader("Location", String("http://") + WiFi.softAPIP().toString(), true);
    server.send(302, "text/plain", "");
}

void setup() { 
    Serial.begin(115200);
    delay(1000);

    if (!LittleFS.begin(true)) {
        Serial.println("LittleFS Mount Failed!");
        return;
    }
    Serial.println("LittleFS Mounted Successfully");

    Serial.println("\nStarting AP...");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid); 

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

    dnsServer.start(DNS_PORT, "*", myIP);

    server.on("/", handleRoot);
    server.on("/generate_204", handleRoot); // Android captive portal
    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP & DNS servers started");
}

void loop() {
    dnsServer.processNextRequest();
    server.handleClient();
    Monitoring::tick();
}
