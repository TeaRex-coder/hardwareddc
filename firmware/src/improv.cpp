#include <ESPmDNS.h>
#include <Preferences.h>
#include <WiFi.h>
#include "../include/improv.h"
#include "../lib/improv/ImprovWiFiLibrary.h"

WiFiServer server(3485);
ImprovWiFi improvSerial(&Serial);
Preferences preferences;

void connectToSavedWiFi(const char* ssid, const char* password);

void wifiSetup() {
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    improvSerial.setDeviceInfo(ImprovTypes::ChipFamily::CF_ESP32, "ImprovWiFiLib", "1.0.0",
                               "BasicWebServer", "http://{LOCAL_IPV4}?name=Guest");
    improvSerial.onImprovError(onImprovWiFiErrorCb);
    improvSerial.onImprovConnected(onImprovWiFiConnectedCb);
    improvSerial.setCustomConnectWiFi(connectWifi); // Optional

    preferences.begin("wifi", false);
    String saved_ssid = preferences.getString("ssid", "");
    String saved_password = preferences.getString("password", "");

    if (saved_ssid.length() > 0) {
        connectToSavedWiFi(saved_ssid.c_str(), saved_password.c_str());
    }
}

void wifiLoop() {
    improvSerial.handleSerial();
}

void onImprovWiFiErrorCb(ImprovTypes::Error err) {
    server.stop();
}

void onImprovWiFiConnectedCb(const char* ssid, const char* password) {
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    server.begin();

    MDNS.begin("hardwareddc");
}

bool connectWifi(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);

    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    }

    if (WiFi.status() == WL_CONNECTED) {
        MDNS.begin("hardwareddc");
    }

    return WiFi.status() == WL_CONNECTED;
}

void connectToSavedWiFi(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);

    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    }

    if (WiFi.status() == WL_CONNECTED) {
        server.begin();

        MDNS.begin("hardwareddc");
    }
}
