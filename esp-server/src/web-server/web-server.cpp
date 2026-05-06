#include "./web-server.h"

namespace AppWebServer {
    namespace {
        static const char* ssid = "NAVCART_DEMO";
        static const byte DNS_PORT = 53;

        static DNSServer dnsServer;
        static WebServer server(80);

    }
    
    String getContentType(String filename);
    void handleRoot();
    void handleNotFound();
    
    DNSServer& getDnsServer() {
        return dnsServer;
    }
    WebServer& getWebServer() {
        return server;
    }

    void setup() {
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

        server.on("/", AppWebServer::handleRoot);
        server.on("/generate_204", AppWebServer::handleRoot); // Android captive portal
        server.onNotFound(AppWebServer::handleNotFound);

        server.begin();
        Serial.println("HTTP & DNS servers started");
    }

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
            server.streamFile(file, AppWebServer::getContentType(path));
            file.close();
            return;
        }
        server.sendHeader("Location", String("http://") + WiFi.softAPIP().toString(), true);
        server.send(302, "text/plain", "");
    }
}
