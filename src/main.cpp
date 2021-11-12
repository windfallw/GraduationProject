#include "file.h"
#include "tof.h"
#include "lv_tft.h"

SKPTOFLIDAR skp1 = SKPTOFLIDAR(&Serial1, 115200, 14, 15);

void setup()
{
    Serial.begin(115200);
    set_littlefs();
    skp1.start();

    set_disp_drv();
    set_rotary_encoder();

    listDir(LITTLEFS, "/", 3);
}

void loop()
{
    skp1.read();
    lv_timer_handler();
}
