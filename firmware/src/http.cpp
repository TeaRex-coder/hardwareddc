#include <Esp.h>
#include <WiFi.h>
#include "../include/http.h"
#include "../lib/ddc/ddc.h"

extern WiFiServer server;
extern DDC ddc;

char linebuf[80];
int charcount = 0;

void handleSetBrightness(String request, WiFiClient& client);
void handleGetBrightness(WiFiClient& client);
void handleSetSource(String request, WiFiClient& client);

void httpSetup() {
    server.begin();
}

void httpLoop() {
    WiFiClient client = server.available();
    if (client) {
        handleHttpRequest(client);
    }
}

void handleHttpRequest(WiFiClient& client) {
    memset(linebuf, 0, sizeof(linebuf));
    charcount = 0;
    boolean currentLineIsBlank = true;

    while (client.connected()) {
        if (client.available()) {
            char c = client.read();
            linebuf[charcount] = c;
            if (charcount < sizeof(linebuf) - 1) {
                charcount++;
            }

            if (c == '\n' && currentLineIsBlank) {
                String request = String(linebuf);
                if (request.startsWith("GET /brightness?value=")) {
                    handleSetBrightness(request, client);
                } else if (request.startsWith("GET /brightness")) {
                    handleGetBrightness(client);
                } else if (request.startsWith("GET /set_source?value=")) {
                    handleSetSource(request, client);
                } else {
                    client.println("HTTP/1.1 404 Not Found");
                    client.println("Content-Type: application/json");
                    client.println("Connection: close");
                    client.println();
                    client.println(
                        "{\"status\":\"error\", \"message\":\"endpoint doesn't exist\"}");
                }
                break;
            }
        }
    }
    delay(1);
    client.stop();
}

void handleSetBrightness(String request, WiFiClient& client) {
    int valueStart = request.indexOf('=') + 1;
    int valueEnd = request.indexOf(' ', valueStart);
    if (valueEnd == -1) {
        valueEnd = request.length();
    }
    String valueStr = request.substring(valueStart, valueEnd);
    int brightnessValue = valueStr.toInt();

    if (brightnessValue >= 0 && brightnessValue <= 100) {
        ddc.setBrightness(brightnessValue);
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: application/json");
        client.println("Connection: close");
        client.println();
        client.println("{\"status\":\"success\", \"brightness\":" + String(brightnessValue) + "}");
    } else {
        client.println("HTTP/1.1 400 Bad Request");
        client.println("Content-Type: application/json");
        client.println("Connection: close");
        client.println();
        client.println("{\"status\":\"error\", \"message\":\"brightness has to be from 0-100\"}");
    }
}

void handleGetBrightness(WiFiClient& client) {
    int currentBrightness = ddc.getBrightness();
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();
    client.println("{\"status\":\"success\", \"current_brightness\":" + String(currentBrightness) +
                   "}");
}

void handleSetSource(String request, WiFiClient& client) {
    int valueStart = request.indexOf('=') + 1;
    int valueEnd = request.indexOf(' ', valueStart);
    if (valueEnd == -1) {
        valueEnd = request.length();
    }
    String valueStr = request.substring(valueStart, valueEnd);
    unsigned int sourceValue = (unsigned int)strtol(valueStr.c_str(), NULL, 16);

    if (sourceValue == 0x0f || sourceValue == 0x10 || sourceValue == 0x11 || sourceValue == 0x12) {
        ddc.setSource(sourceValue);
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: application/json");
        client.println("Connection: close");
        client.println();
        client.println("{\"status\":\"success\", \"source\":\"0x" + String(sourceValue, HEX) +
                       "\"}");
    } else {
        client.println("HTTP/1.1 400 Bad Request");
        client.println("Content-Type: application/json");
        client.println("Connection: close");
        client.println();
        client.println("{\"status\":\"error\", \"message\":\"invalid source value\"}");
    }
}
