#include "netsrv.h"

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
    String id = String((uint32_t)ESP.getEfuseMac(), HEX);
    id.toUpperCase();
    acConfig.apid = "ESP-" + id;
    Serial.println(acConfig.apid);
    acConfig.psk = "12345678";
    Portal.config(acConfig);

    Server.on("/", rootPage);
    if (Portal.begin())
    {
        Serial.println("WiFi connected: " + WiFi.localIP().toString());
    }
}