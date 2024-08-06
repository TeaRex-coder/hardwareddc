#include <ESPmDNS.h>
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

    String mfg = ddc.getMfg();
    String model = ddc.getModel();
    String product = ddc.getProduct();
    String serial = ddc.getSerial();
    uint32_t serialDecimal = ddc.getSerialDecimal();
    String year = String(ddc.getYear());

    MDNS.addServiceTxt("_ddcutil", "_tcp", "1:year", year.c_str());
    MDNS.addServiceTxt("_ddcutil", "_tcp", "1:serial", String(serialDecimal).c_str());
    MDNS.addServiceTxt("_ddcutil", "_tcp", "1:product", product.c_str());
    MDNS.addServiceTxt("_ddcutil", "_tcp", "1:model", model.c_str());
    MDNS.addServiceTxt("_ddcutil", "_tcp", "1:mfg", mfg.c_str());
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
