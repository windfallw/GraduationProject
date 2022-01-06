#include "file.h"
#include "tof.h"
#include "lv_tft.h"
#include "net_service.h"

SKPTOFLIDAR skp1 = SKPTOFLIDAR(&Serial1, 921600, 15, 14);
SKPTOFLIDAR skp2 = SKPTOFLIDAR(&Serial2, 921600, 21, 13);

void setup()
{
    Serial.begin(115200);

    set_littlefs();

    skp1.start();
    skp2.start();

    set_disp_drv();
    set_rotary_encoder();
    set_ui();

    set_netsrv();

    // writeConfigFile();
    // readConfigFile();
    // Serial.printf("%s %d \r\n", config.hostname, config.limit);
    // readFile("/config.json");
    listDir("/", 3);
}

void update_ui()
{
    lv_label_set_text_fmt(cz_label, "当前: %d mm %d mm", skp1.distance, skp2.distance);
}

void loop()
{
    skp1.read_handler();
    skp2.read_handler();

    update_ui();
    lv_timer_handler();

    dnsServer.processNextRequest();
}
