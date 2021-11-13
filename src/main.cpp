#include "file.h"
#include "tof.h"
#include "lv_tft.h"

SKPTOFLIDAR skp1 = SKPTOFLIDAR(&Serial1, 921600, 15, 14);

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
    skp1.read_handler();
    lv_timer_handler();
}
