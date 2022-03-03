#ifndef FILEH
#define FILEH

#include <Arduino.h>
#include "LITTLEFS.h"

struct
{
    char hostname[64];
    int limit;
} config = {"example.com", 1000};

extern void set_littlefs();
extern void listDir(const char *dirname, uint8_t levels);
extern void readFile(const char *path);
extern void writeFile(const char *path);
extern void readConfigFile();
extern void writeConfigFile();

#endif