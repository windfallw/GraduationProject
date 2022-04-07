#include "file.h"
#include "tof.h"
#include "IP5108.h"
#include "lvtft.hpp"
#include "netsrv.h"

TaskHandle_t Task1;
TaskHandle_t Task2;
TaskHandle_t Task3;

SKPTOFLIDAR skp1 = SKPTOFLIDAR(1, &Serial1, 921600, 15, 14); // rx1 15 tx1 14
SKPTOFLIDAR skp2 = SKPTOFLIDAR(2, &Serial2, 921600, 16, 17); // rx2 16 tx2 17

IP5108 bms = IP5108(&Wire, 21, 22, 400000); // SDA1 21 SCL1 22

/* pin channel(0-15) resolution(1-16)  freq
All pins that can act as outputs can be used as PWM pins.
timerNum = 0 Pin = 13 channel = 0 resolution = 8 */
shinelight buzzer = shinelight(0, 13, 0);

void update_ui()
{
    // lv_scr_load_anim(chargeScreen, LV_SCR_LOAD_ANIM_FADE_ON, 100, 50, false);
    // if (lv_scr_act() == mainScreen)
    // {
    //     lv_label_set_text_fmt(wifi_label, "STA %s | AP %s", WiFi.SSID().c_str(), WiFi.softAPSSID().c_str());
    //     lv_label_set_text_fmt(battery_label, "%d%%", bms.percent);

    //     if (bms.percent >= 80)
    //         lv_label_set_text(battery_ico, LV_SYMBOL_BATTERY_FULL);
    //     else if (bms.percent >= 60)
    //         lv_label_set_text(battery_ico, LV_SYMBOL_BATTERY_3);
    //     else if (bms.percent >= 40)
    //         lv_label_set_text(battery_ico, LV_SYMBOL_BATTERY_2);
    //     else if (bms.percent >= 20)
    //         lv_label_set_text(battery_ico, LV_SYMBOL_BATTERY_1);
    //     else
    //         lv_label_set_text(battery_ico, LV_SYMBOL_BATTERY_EMPTY);

    //     if (bms.isCharging)
    //     {
    //         lv_label_set_text(battery_cg_ico, "#00FF00 " LV_SYMBOL_CHARGE);
    //         lv_obj_clear_flag(battery_cg_ico, LV_OBJ_FLAG_HIDDEN);
    //     }
    //     else
    //         lv_obj_add_flag(battery_cg_ico, LV_OBJ_FLAG_HIDDEN);

    //     lv_label_set_text_fmt(tof_label, "当前: %d mm %d mm", skp1.distance, skp2.distance);
    //     lv_label_set_text_fmt(bt_label, "%d mA %d mV %d mV %d", bms.current, bms.voltage, bms.voltageOc, bms.State);

    //     mainScreenAlign();
    // }
}

void Task1TOF(void *pvParameters)
{
    Serial.printf("%s running on core %d with priority %d\r\n", pcTaskGetTaskName(NULL), xPortGetCoreID(), uxTaskPriorityGet(NULL));
    for (;;)
    {
        if (skp1.handler(cg.alarm.tof1) || skp2.handler(cg.alarm.tof2))
            buzzer.open();

        vTaskDelay(1); // vTaskDelay() = delay()
    }
}

void Task2Scan(void *pvParameters)
{
    Serial.printf("%s running on core %d with priority %d\r\n", pcTaskGetTaskName(NULL), xPortGetCoreID(), uxTaskPriorityGet(NULL));
    for (;;)
    {
        if (buzzer.ReqOff)
        {
            buzzer.close();
            if (mqttClient.connected())
            {
                for (uint8_t tid = 0; tid <= TOFDEVICENUMBER; tid++)
                {
                    if (alog[tid].onpub)
                    {
                        String output;
                        StaticJsonDocument<256> doc;
                        doc["log"] = true;
                        doc["mac"] = cg.mqtt.macddr;
                        doc["tid"] = tid;
                        doc["distance"] = alog[tid].distance;
                        doc["limit"] = alog[tid].limit;
                        serializeJson(doc, output);
                        mqttClient.publish(cg.mqtt.publish.c_str(), 0, false, output.c_str());
                        alog[tid].onpub = false;
                    }
                }
            }
            buzzer.ReqOff = false;
        }

        bms.update();
        vTaskDelay(1);
    }
}

void Task3TFT(void *pvParameters)
{
    Serial.printf("%s running on core %d with priority %d\r\n", pcTaskGetTaskName(NULL), xPortGetCoreID(), uxTaskPriorityGet(NULL));
    // TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;)
    {
        update_ui();
        lv_task_handler();
        vTaskDelay(1);
        // vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1));
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    set_littlefs();

    skp1.start();
    skp2.start();

    bms.setup();

    set_lvgl();

    /*  defalut ESP_TASK_PRIO_MAX = configMAX_PRIORITIES = 25
    but priority value range (0-24) set priority = -1 or bigger than 24 will auto correct to 24*/
    xTaskCreatePinnedToCore(
        Task1TOF,   /* Task function. */
        "Task1ToF", /* name of task. */
        10240,      /* Stack size of task */
        NULL,       /* parameter of the task */
        3,          /* priority of the task */
        &Task1,     /* Task handle to keep track of created task */
        0);         /* pin task to core 0 */

    xTaskCreatePinnedToCore(
        Task2Scan,   /* Task function. */
        "Task2Scan", /* name of task. */
        10240,       /* Stack size of task */
        NULL,        /* parameter of the task */
        2,           /* priority of the task */
        &Task2,      /* Task handle to keep track of created task */
        0);          /* pin task to core 0 */

    xTaskCreatePinnedToCore(
        Task3TFT,   /* Task function. */
        "Task3TFT", /* name of task. */
        10240,      /* Stack size of task */
        NULL,       /* parameter of the task */
        1,          /* priority of the task */
        &Task3,     /* Task handle to keep track of created task */
        0);         /* pin task to core 1 */

    set_netsrv();      // try connect wifi & set up AP&OTA&Webserver
    set_mqtt();        // mqtt client
    set_checkTimer(1); // wifi check timer

    Serial.println("setup done");

    Serial.printf("%s running on core %d with priority %d\r\n", pcTaskGetTaskName(NULL), xPortGetCoreID(), uxTaskPriorityGet(NULL));
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
