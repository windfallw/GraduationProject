#ifndef NETSRV
#define NETSRV

#include <WiFi.h>
#include <DNSServer.h>
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"

extern DNSServer dnsServer;
extern AsyncWebServer server;
extern void set_netsrv();

#endif