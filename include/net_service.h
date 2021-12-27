#ifndef NETSRV
#define NETSRV

#include <DNSServer.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"

extern DNSServer dnsServer;
extern AsyncWebServer server;
extern void set_netsrv();

#endif