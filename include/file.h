#ifndef FILEH
#define FILEH

#include "FS.h"
#include <LITTLEFS.h>

#include <ArduinoJson.h>

// writeConfigFile();
// readConfigFile();
// Serial.printf("%s %d \r\n", config.hostname, config.limit);
// readFile("/config.json");
// listDir("/", 3);

struct
{
    char hostname[64];
    int limit;
} config = {"example.com", 1000};

extern void set_littlefs();
extern void listDir(const char *dirname, uint8_t levels);
extern void readFile(const char *filename);
extern bool readConfigFile();
extern bool writeConfigFile();

#endif