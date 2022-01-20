#include "net_service.h"

WebServer Server;
AutoConnect Portal(Server);
AutoConnectConfig acConfig;

void rootPage()
{
    char content[] = "Hello, world";
    Server.send(200, "text/plain", content);
}

void set_netsrv()
{
    // acConfig.apid = "ESP-" + String((uint32_t)(ESP.getEfuseMac() >> 32), HEX);
    // Serial.println(acConfig.apid);
    acConfig.psk = "12345678";
    Portal.config(acConfig);

    Server.on("/", rootPage);
    if (Portal.begin())
    {
        Serial.println("WiFi connected: " + WiFi.localIP().toString());
    }
}