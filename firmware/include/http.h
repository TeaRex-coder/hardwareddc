#ifndef HTTP_H
#define HTTP_H

#include <WiFi.h>

void handleHttpRequest(WiFiClient& client);
void httpSetup();
void httpLoop();

#endif