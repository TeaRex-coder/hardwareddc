#include "http.h"
#include <WiFi.h>
#include <Esp.h>
#include "ddc.h"

extern WiFiServer server;
extern DDC ddc;

char linebuf[80];
int charcount = 0;

void httpSetup() {
    server.begin();
}

void httpLoop() {
    WiFiClient client = server.available();
    if (client) {
        handleHttpRequest(client);
    }
}

void handleHttpRequest(WiFiClient &client) {
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
                    int valueStart = request.indexOf('=') + 1;
                    int valueEnd = request.indexOf(' ', valueStart);
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
                        client.println("HTTP/1.1 400 bad request");
                        client.println("Content-Type: application/json");
                        client.println("Connection: close");
                        client.println();
                        client.println("{\"status\":\"error\", \"message\":\"brightness has to be from 0-100\"}");
                    }
                } else {
                    client.println("HTTP/1.1 404 not found");
                    client.println("Content-Type: application/json");
                    client.println("Connection: close");
                    client.println();
                    client.println("{\"status\":\"error\", \"message\":\"endpoint doesn't exist\"}");
                }
                break;
            }
        }
    }
    delay(1);
    client.stop();
}
