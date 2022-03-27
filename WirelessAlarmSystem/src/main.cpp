#include "file.h"
#include "tof.h"
#include "IP5108.h"
#include "lv_tft.h"
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
    lv_label_set_text_fmt(tof_label, "当前: %d mm %d mm", skp1.distance, skp2.distance);
    // lv_label_set_text_fmt(battery_level_label, "%d mA %d mV %d mV", bms.current, bms.voltage, bms.voltageOc);
}

void Task1code(void *pvParameters)
{
    Serial.printf("%s running on core %d with priority %d\r\n", pcTaskGetTaskName(NULL), xPortGetCoreID(), uxTaskPriorityGet(NULL));
    for (;;)
    {
        if (skp1.handler(cg.alarm.tof1) || skp2.handler(cg.alarm.tof2))
            buzzer.open();

        vTaskDelay(1); // vTaskDelay() = delay()
    }
}

void Task2code(void *pvParameters)
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

void Task3code(void *pvParameters)
{
    Serial.printf("%s running on core %d with priority %d\r\n", pcTaskGetTaskName(NULL), xPortGetCoreID(), uxTaskPriorityGet(NULL));
    // TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;)
    {
        update_ui();
        lv_timer_handler();
        vTaskDelay(1);
        // vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1));
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    Serial.printf("%s running on core %d with priority %d\r\n", pcTaskGetTaskName(NULL), xPortGetCoreID(), uxTaskPriorityGet(NULL));

    set_littlefs();

    skp1.start();
    skp2.start();

    bms.set_up();

    set_rotary_encoder();
    set_disp_drv();
    set_ui();

    /*  defalut ESP_TASK_PRIO_MAX = configMAX_PRIORITIES = 25
    but priority value range (0-24) set priority = -1 or bigger than 24 will auto correct to 24*/
    xTaskCreatePinnedToCore(
        Task1code, /* Task function. */
        "Task1",   /* name of task. */
        10240,     /* Stack size of task */
        NULL,      /* parameter of the task */
        3,         /* priority of the task */
        &Task1,    /* Task handle to keep track of created task */
        0);        /* pin task to core 0 */

    xTaskCreatePinnedToCore(
        Task2code, /* Task function. */
        "Task2",   /* name of task. */
        10240,     /* Stack size of task */
        NULL,      /* parameter of the task */
        3,         /* priority of the task */
        &Task2,    /* Task handle to keep track of created task */
        0);        /* pin task to core 0 */

    xTaskCreatePinnedToCore(
        Task3code, /* Task function. */
        "Task3",   /* name of task. */
        10240,     /* Stack size of task */
        NULL,      /* parameter of the task */
        3,         /* priority of the task */
        &Task3,    /* Task handle to keep track of created task */
        0);        /* pin task to core 0 */

    set_netsrv();      // try connect wifi & set up AP&OTA&Webserver
    set_mqtt();        // mqtt client
    set_checkTimer(1); // wifi check timer

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
