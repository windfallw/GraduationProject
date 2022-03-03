#include "file.h"
#include "tof.h"
#include "IP5108.h"
#include "lv_tft.h"
#include "netsrv.h"

TaskHandle_t Task1;

SKPTOFLIDAR skp1 = SKPTOFLIDAR(&Serial1, 921600, 15, 14); // rx1 15 tx1 14
SKPTOFLIDAR skp2 = SKPTOFLIDAR(&Serial2, 921600, 16, 17); // rx2 16 tx2 17

IP5108 bms = IP5108(&Wire, 21, 22, 400000); // SDA1 21 SCL1 22

/* pin channel(0-15) resolution(1-16)  freq
All pins that can act as outputs can be used as PWM pins. */
shinelight buzzer = shinelight(13, 0, 16, 50000);

void update_ui()
{
    lv_label_set_text_fmt(cz_label, "当前: %d mm %d mm", skp1.distance, skp2.distance);
    lv_label_set_text_fmt(battery_level_label, "%d mA %d mV %d mV", (int)round(bms.current), (int)round(bms.voltage), (int)round(bms.voltageOc));
    // Serial.printf("%d mA %d mV %d mV\r\n", (int)round(bms.current), (int)round(bms.voltage), (int)round(bms.voltageOc));
}

void Task1code(void *pvParameters)
{
    Serial.printf("Task1 running on core %d\r\n", xPortGetCoreID());
    for (;;)
    {
        skp1.read_handler();
        skp2.read_handler();
        bms.update();

        update_ui();
        lv_timer_handler();
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    Serial.printf("Arduino Core running on core %d\r\n", xPortGetCoreID());

    set_littlefs();

    skp1.start();
    skp2.start();

    bms.set_up();

    set_disp_drv();
    set_rotary_encoder();
    set_ui();

    xTaskCreatePinnedToCore(
        Task1code, /* Task function. */
        "Task1",   /* name of task. */
        10240,     /* Stack size of task */
        NULL,      /* parameter of the task */
        1,         /* priority of the task */
        &Task1,    /* Task handle to keep track of created task */
        0);        /* pin task to core 0 */

    set_netsrv();
    Serial.println("setup done");
}

void loop()
{
    ArduinoOTA.handle();
}
