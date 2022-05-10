#ifndef FILEH
#define FILEH

#include <Arduino.h>

typedef struct
{
    String ssid;
    String pwd;
} sta_cg_t;

typedef struct
{
    String ssid;
    String pwd;
} ap_cg_t;

typedef struct
{
    uint32_t tofMax; // hard coding maximum limit 20m
    uint32_t tofMin; // hard coding minimum limit 0m
    uint32_t tof1;
    uint32_t tof2;

    uint32_t msMax; // hard coding maximum 10s
    uint32_t msMin; // hard coding minimum 0s
    uint32_t ms;

    uint32_t freq;
    uint32_t dutyCycle;
} alarm_cg_t;

typedef struct
{
    String macddr; // hard coding
    String user;
    String pwd;
    String subscribe;
    String publish;
    String server;
    uint16_t port;
} mqtt_cg_t;

typedef struct
{
    sta_cg_t sta[3];
    ap_cg_t ap;
    alarm_cg_t alarm;
    mqtt_cg_t mqtt;
    String stream;
    char *convert(String str)
    {
        return const_cast<char *>(str.c_str());
    }
} sys_cg_t;

typedef struct
{
    String ssid;
    String pwd;
    bool web = false;                        // for web
    bool wifi = false;                       // wifi check flag
    bool lvgl = false;                       // lvgl check flag
    const uint64_t checkMicroSec = 10000000; // use timer to check wifi status per 10s
} sys_signal_t;

extern sys_cg_t syscg;

extern sys_signal_t signal;

void set_littlefs();
void listDir(const char *dirname, uint8_t levels);
void readFile(const char *path);
void writeFile(const char *path);

void readConfigFile();
void writeConfigFile();
void resetConfigFile();

#endif