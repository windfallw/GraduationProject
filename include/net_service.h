#ifndef NETSRV
#define NETSRV

#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnect.h>

extern WebServer Server;
extern AutoConnect Portal;
extern AutoConnectConfig acConfig;

extern void set_netsrv();

#endif