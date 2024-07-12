#include "../include/improv.h"
#include "../include/http.h"
#include "../lib/ddc/ddc.h"

DDC ddc;

void brightnessSetup() {
    Serial.begin(9600);
    while (!ddc.begin()) {
        Serial.print(F("DDC error. Trying again in 5 sec.\n"));
        delay(5000);
    }
    Serial.print(F("DDC found.\n"));
}

void setup() {
    Wire.begin();
    wifiSetup();
    if (improvSerial.isConnected()) {
        httpSetup();
        brightnessSetup();
    }
}

void loop() {
    wifiLoop();
    if (improvSerial.isConnected()) {
        httpLoop();
    }
}
