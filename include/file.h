#ifndef FILEH
#define FILEH

#include "FS.h"
#include <LITTLEFS.h>

#ifndef CONFIG_LITTLEFS_FOR_IDF_3_2
#include <time.h>
#endif

extern void set_littlefs();
extern void listDir(fs::FS &fs, const char *dirname, uint8_t levels);

#endif