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

    writeConfigFile();
    readConfigFile();
    Serial.printf("%s %d \r\n", config.hostname, config.limit);
    readFile("/config.json");
    listDir("/", 3);
}

void loop()
{
    skp1.read_handler();
    lv_label_set_text_fmt(cz_label, "当前: %d mm", skp1.distance);

    lv_timer_handler();
}
