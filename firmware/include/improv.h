#ifndef IMPROV_H
#define IMPROV_H

#include "ImprovWiFiLibrary.h"

extern ImprovWiFi improvSerial;

void wifiSetup();
void wifiLoop();
void onImprovWiFiErrorCb(ImprovTypes::Error err);
void onImprovWiFiConnectedCb(const char* ssid, const char* password);
bool connectWifi(const char* ssid, const char* password);

#endif
