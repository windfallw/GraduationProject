#include "file.h"
#include "tof.h"
#include "IP5108.h"
#include "lv_tft.h"
#include "net_service.h"

SKPTOFLIDAR skp1 = SKPTOFLIDAR(&Serial1, 921600, 15, 14); // rx1 15 tx1 14
SKPTOFLIDAR skp2 = SKPTOFLIDAR(&Serial2, 921600, 16, 17); // rx2 16 tx2 17

IP5108 bms = IP5108(&Wire, 21, 22, 400000); // SDA1 21 SCL1 22

TaskHandle_t Task1;

void Task1code(void *pvParameters)
{
    for (;;)
    {
        Portal.handleClient();
    }
}

void setup()
{
    Serial.begin(115200);

    set_littlefs();

    skp1.start();
    skp2.start();

    set_disp_drv();
    set_rotary_encoder();
    set_ui();

    // pinMode(21, PULLUP);
    // pinMode(22, PULLUP);

    bms.set_up();

    // set_netsrv();

    listDir("/", 3);

    // xTaskCreatePinnedToCore(
    //     Task1code, /* Task function. */
    //     "Task1",   /* name of task. */
    //     10000,     /* Stack size of task */
    //     NULL,      /* parameter of the task */
    //     1,         /* priority of the task */
    //     &Task1,    /* Task handle to keep track of created task */
    //     0);        /* pin task to core 0 */
}

void update_ui()
{
    lv_label_set_text_fmt(cz_label, "当前: %d mm %d mm", skp1.distance, skp2.distance);
    lv_label_set_text_fmt(battery_level_label, "%d mA %d mV %d mV", (int)round(bms.current), (int)round(bms.voltage), (int)round(bms.voltageOc));
    // Serial.printf("%d mA %d mV %d mV\r\n", (int)round(bms.current), (int)round(bms.voltage), (int)round(bms.voltageOc));
}

void loop()
{
    skp1.read_handler();
    skp2.read_handler();

    update_ui();
    lv_timer_handler();

    bms.update();
}
