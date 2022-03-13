#include "file.h"
#include "tof.h"
#include "IP5108.h"
#include "lv_tft.h"
#include "netsrv.h"

TaskHandle_t Task1;

SKPTOFLIDAR skp1 = SKPTOFLIDAR(1, &Serial1, 921600, 15, 14); // rx1 15 tx1 14
SKPTOFLIDAR skp2 = SKPTOFLIDAR(2, &Serial2, 921600, 16, 17); // rx2 16 tx2 17

IP5108 bms = IP5108(&Wire, 21, 22, 400000); // SDA1 21 SCL1 22

/* pin channel(0-15) resolution(1-16)  freq
All pins that can act as outputs can be used as PWM pins. */
shinelight buzzer = shinelight(13, 0, 8, 1);

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
        if (skp1.handler() || skp2.handler())
        {
            // buzzer.writeCycle(30);
            // mqttClient.publish(cg.mqtt.publish.c_str(), 0, false, "{}");
            Serial.printf("trigger alarm tof1: %d tof2: %d\r\n", skp1.distance, skp2.distance);
        }

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

    set_netsrv(); // try connect wifi & set up AP&OTA&Webserver
    set_mqtt();   // mqtt client

    Serial.println("setup done");
}

void loop()
{
    ArduinoOTA.handle();
    dnsServer.processNextRequest();

    if (CONN_SIGN.WEB)
    {
        // only exec once.
        CONN_SIGN.WEB = false;
        if (!conn_wifi(CONN_SIGN.ssid, CONN_SIGN.pwd))
        {
            // connect fail rollback to known wifi.
            conn_wifi(false);
        }
    }

    if (CONN_SIGN.WiFi)
    {
        CONN_SIGN.WiFi = false;
        if (!WiFi.isConnected())
            conn_wifi(true);
    }

    if (CONN_SIGN.MQTT && WiFi.isConnected())
    {
        CONN_SIGN.MQTT = false;
        mqttClient.connect();
    }
}
