#include "../include/http.h"
#include "../include/improv.h"
#include "../lib/ddc/ddc.h"

DDC ddc;
bool ddcConnected = false;

void ddcSetup() {
    Serial.begin(115200);
    while (!ddc.begin()) {
        Serial.print(F("DDC didn't connect, wait 5 sec\n"));
        delay(5000);
    }
    Serial.print(F("DDC found\n"));
    ddcConnected = true;
}

void setup() {
    Wire.begin();
    wifiSetup();
    if (improvSerial.isConnected()) {
        httpSetup();
        ddcSetup();
    }
}

void loop() {
    wifiLoop();
    if (improvSerial.isConnected()) {
        httpLoop();
    }
}
