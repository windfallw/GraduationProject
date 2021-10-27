#include "lv_tft.h"

// #include "FS.h"
// #include <LITTLEFS.h>

// #define FORMAT_LITTLEFS_IF_FAILED true

void setup()
{
    Serial.begin(115200);
    set_disp_drv();
    // if (!LITTLEFS.begin(FORMAT_LITTLEFS_IF_FAILED))
    // {
    //     Serial.println("LITTLEFS Mount Failed");
    //     return;
    // }
}

void loop()
{
    lv_timer_handler();
}
