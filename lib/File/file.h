#ifndef FILEH
#define FILEH

#include <Arduino.h>
#include "LITTLEFS.h"
#include "ArduinoJson.h"

struct sta_config
{
    String ssid;
    String pwd;
};

struct ap_config
{
    String ssid;
    String pwd;
};

struct alarm_config
{
    uint32_t tof1;
    uint32_t tof2;
    uint32_t ms;
};

struct mqtt_config
{
    String subscribe;
    String publish;
    String server;
    uint16_t port;
};

struct cg_t
{
    struct sta_config sta[3];
    struct ap_config ap;
    struct alarm_config alarm;
    struct mqtt_config mqtt;
    String stream;
    char *convert(String str)
    {
        return const_cast<char *>(str.c_str());
    }
};

struct conn_t
{
    String ssid;
    String pwd;
    bool WIFI = false;
};

extern struct cg_t cg;
extern struct conn_t I_WANT_CONN;

extern void set_littlefs();
extern void listDir(const char *dirname, uint8_t levels);
extern void readFile(const char *path);
extern void writeFile(const char *path);

extern void readConfigFile();
extern void writeConfigFile();
extern void resetConfigFile();

#endif