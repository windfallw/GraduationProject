#ifndef FILEH
#define FILEH

#include <Arduino.h>
#include "LITTLEFS.h"
#include "ArduinoJson.h"

typedef struct
{
    String ssid;
    String pwd;
} sta_config;

typedef struct
{
    String ssid;
    String pwd;
} ap_config;

typedef struct
{
    uint32_t tofMax; // hard coding maximum limit 20m
    uint32_t tof1;
    uint32_t tof2;
    uint32_t msMax; // hard coding maximum 10s
    uint32_t ms;
    uint32_t freq;
    uint32_t dutyCycle;
} alarm_config;

typedef struct
{
    String macddr; // hard coding
    String user;
    String pwd;
    String subscribe;
    String publish;
    String server;
    uint16_t port;
} mqtt_config;

typedef struct
{
    sta_config sta[3];
    ap_config ap;
    alarm_config alarm;
    mqtt_config mqtt;
    String stream;
    char *convert(String str)
    {
        return const_cast<char *>(str.c_str());
    }
} config_sys_t;

typedef struct
{
    String ssid;
    String pwd;
    bool web = false;                        // for web
    bool wifi = false;                       // wifi check flag
    bool mqtt = false;                       // mqtt check flag
    const uint64_t checkMicroSec = 10000000; // use timer to check wifi status per 10s
} conn_sign_t;

extern config_sys_t sysconfig;

extern conn_sign_t conn_sign;

void set_littlefs();
void listDir(const char *dirname, uint8_t levels);
void readFile(const char *path);
void writeFile(const char *path);

void readConfigFile();
void writeConfigFile();
void resetConfigFile();

#endif