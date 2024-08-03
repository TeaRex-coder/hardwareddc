#include <Esp.h>
#include <WiFi.h>
#include "../include/http.h"
#include "../lib/ddc/ddc.h"

extern WiFiServer server;
extern DDC ddc;
extern bool ddcConnected;

char linebuf[80];
int charcount = 0;

void handleDisplays(WiFiClient& client);
void handleSetSmooth(WiFiClient& client, int startValue, int endValue);
void handleSmoothStatus(WiFiClient& client);
void handleGetBrightness(WiFiClient& client);
void handleSetBrightness(WiFiClient& client, int value);
void handleDDCStatus(WiFiClient& client);
void handleDisplayPower(WiFiClient& client);
void handleDisplayPowerValue(WiFiClient& client);
void handleVersion(WiFiClient& client);
void handleUpdate(WiFiClient& client);
void handleFullUpdate(WiFiClient& client);

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

                for (int i = 0; i < request.length(); i++) {
                    request[i] = tolower(request[i]);
                }

                if (request.startsWith("get /displays")) {
                    handleDisplays(client);
                } else if (request.startsWith("get /smooth/1/brightness/")) {
                    int startIndex = request.indexOf("/smooth/1/brightness/") +
                                     String("/smooth/1/brightness/").length();
                    int endIndex = request.indexOf("/", startIndex);

                    String brightnessValueOneStr = request.substring(startIndex, endIndex);
                    int brightnessValueOne = brightnessValueOneStr.toInt();

                    startIndex = endIndex + 1;
                    endIndex = request.indexOf(" ", startIndex);
                    String brightnessValueTwoStr = request.substring(startIndex, endIndex);
                    int brightnessValueTwo = brightnessValueTwoStr.toInt();

                    handleSetSmooth(client, brightnessValueOne, brightnessValueTwo);
                } else if (request.startsWith("get /smooth/1")) {
                    handleSmoothStatus(client);
                } else if (request.startsWith("get /1/brightness/")) {
                    int startIndex =
                        request.indexOf("/1/brightness/") + String("/1/brightness/").length();
                    int endIndex = request.indexOf(" ", startIndex);

                    if (endIndex > startIndex) {
                        String brightnessValueStr = request.substring(startIndex, endIndex);
                        int brightnessValue = brightnessValueStr.toInt();
                        handleSetBrightness(client, brightnessValue);
                    } else {
                        handleGetBrightness(client);
                    }
                } else if (request.startsWith("get /1/brightness")) {
                    handleGetBrightness(client);
                } else if (request.startsWith("get /1")) {
                    handleDDCStatus(client);
                } else if (request.startsWith("get /display-power/")) {
                    handleDisplayPowerValue(client);
                } else if (request.startsWith("get /display-power")) {
                    handleDisplayPower(client);
                } else if (request.startsWith("get /version")) {
                    handleVersion(client);
                } else if (request.startsWith("get /update")) {
                    handleUpdate(client);
                } else if (request.startsWith("get /full-update")) {
                    handleFullUpdate(client);
                } else {
                    client.println("HTTP/1.1 400 Error");
                    client.println("Connection: keep-alive");
                    client.println("X-Powered-By: Kemal");
                    client.println("Content-Type: text/html");
                    client.println("Content-Length: 5");
                    client.println();
                    client.println("Error");
                }
                break;
            }

            if (c == '\n') {
                currentLineIsBlank = true;
            } else if (c != '\r') {
                currentLineIsBlank = false;
            }
        }
    }
    delay(1);
    client.stop();
}

void handleDisplays(WiFiClient& client) {
    if (!ddcConnected) {
        client.println("HTTP/1.1 200 OK");
        client.println("Connection: keep-alive");
        client.println("X-Powered-By: Kemal");
        client.println("Content-Type: text/html");
        client.println("Content-Length: 143");
        client.println();
        client.println("No displays found.");
        client.println("Is DDC/CI enabled in the monitor's on screen display?");
        client.println("Run \"ddcutil environment\" to check for system configuration problems.");
        return;
    }
    String mfg = ddc.getMfg();
    String model = ddc.getModel();
    String productCode = ddc.getProduct();
    String productSerial = ddc.getProductSerial();
    uint32_t serialDecimal = ddc.getSerialDecimal();
    String serial = ddc.getSerial();
    uint16_t year = ddc.getYear();
    uint8_t week = ddc.getWeek();
    String vcpVersion = ddc.getVCP();

    client.println("HTTP/1.1 200 OK");
    client.println("Connection: keep-alive");
    client.println("X-Powered-By: Kemal");
    client.println("Content-Type: text/html");
    client.println("Content-Length: 316");
    client.println();

    client.println("Display 1");
    client.println("   I2C bus: /dev/i2c-2");
    client.println("   EDID synopsis:");
    client.println("      Mfg id:               " + mfg);
    client.println("      Model:                " + model);
    client.println("      Product code:         " + productCode);
    client.println("      Serial number:        " + productSerial);
    client.println("      Binary serial number: " + String(serialDecimal) + " (" + serial + ")");
    client.println("      Manufacture year:     " + String(year) + ",  Week: " + String(week));
    client.println("   VCP version:         " + vcpVersion);
    client.println("");
}

void handleSetSmooth(WiFiClient& client, int startValue, int endValue) {
    int step = (startValue < endValue) ? 1 : -1;
    int currentValue = startValue;

    while (currentValue != endValue) {
        ddc.setBrightness(currentValue);
        currentValue += step;
        delay(50);
    }

    ddc.setBrightness(endValue);

    client.println("HTTP/1.1 200 OK");
    client.println("Connection: keep-alive");
    client.println("X-Powered-By: Kemal");
    client.println("Content-Type: text/html");
    client.println("Content-Length: 0");
}

void handleSmoothStatus(WiFiClient& client) {
    if (ddcConnected) {
        client.println("HTTP/1.1 200 OK");
        client.println("Connection: keep-alive");
        client.println("X-Powered-By: Kemal");
        client.println("Content-Type: text/html");
        client.println("Content-Length: 2");
        client.println();
        client.println("OK");
    } else {
        client.println("HTTP/1.1 400 Error");
        client.println("Connection: keep-alive");
        client.println("X-Powered-By: Kemal");
        client.println("Content-Type: text/html");
        client.println("Content-Length: 5");
        client.println();
        client.println("Error");
    }
}

void handleSetBrightness(WiFiClient& client, int value) {
    ddc.setBrightness(value);
    client.println("HTTP/1.1 200 OK");
    client.println("Connection: keep-alive");
    client.println("X-Powered-By: Kemal");
    client.println("Content-Type: text/html");
    client.println("Content-Length: 0");
}

void handleGetBrightness(WiFiClient& client) {
    if (!ddcConnected) {
        client.println("HTTP/1.1 400 Error");
        client.println("Connection: keep-alive");
        client.println("X-Powered-By: Kemal");
        client.println("Content-Type: text/html");
        client.println("Content-Length: 5");
        client.println();
        client.println("Error");
        return;
    }

    int brightness = ddc.getBrightness();

    client.println("HTTP/1.1 200 OK");
    client.println("Connection: keep-alive");
    client.println("X-Powered-By: Kemal");
    client.println("Content-Type: text/html");
    client.println("Content-Length: 3");
    client.println();
    client.println(brightness);
}

void handleDDCStatus(WiFiClient& client) {
    if (ddcConnected) {
        client.println("HTTP/1.1 200 OK");
        client.println("Connection: keep-alive");
        client.println("X-Powered-By: Kemal");
        client.println("Content-Type: text/html");
        client.println("Content-Length: 2");
        client.println();
        client.println("OK");
    } else {
        client.println("HTTP/1.1 400 Error");
        client.println("Connection: keep-alive");
        client.println("X-Powered-By: Kemal");
        client.println("Content-Type: text/html");
        client.println("Content-Length: 5");
        client.println();
        client.println("Error");
    }
}

// Pi specific

void handleDisplayPower(WiFiClient& client) {
    client.println("HTTP/1.1 200 OK");
    client.println("Connection: keep-alive");
    client.println("X-Powered-By: Kemal");
    client.println("Content-Type: text/html");
    client.println("Content-Length: 16");
    client.println();
    client.println("display_power=1");
}

void handleDisplayPowerValue(WiFiClient& client) {
    client.println("HTTP/1.1 200 OK");
    client.println("Connection: keep-alive");
    client.println("X-Powered-By: Kemal");
    client.println("Content-Type: text/html");
    client.println("Content-Length: 0");
}

void handleVersion(WiFiClient& client) {
    client.println("HTTP/1.1 200 OK");
    client.println("Connection: keep-alive");
    client.println("X-Powered-By: Kemal");
    client.println("Content-Type: text/html");
    client.println("Content-Length: 3");
    client.println();
    client.println("1.0");
}

void handleUpdate(WiFiClient& client) {
    client.println("HTTP/1.1 200 OK");
    client.println("Connection: keep-alive");
    client.println("X-Powered-By: Kemal");
    client.println("Content-Type: text/html");
    client.println("Content-Length: 0");
}

void handleFullUpdate(WiFiClient& client) {
    client.println("HTTP/1.1 200 OK");
    client.println("Connection: keep-alive");
    client.println("X-Powered-By: Kemal");
    client.println("Content-Type: text/html");
    client.println("Content-Length: 0");
}