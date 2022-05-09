#include "var.hpp"

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

void resume_tof_measure()
{
    skp1.pause = false;
    skp2.pause = false;
}

void pause_tof_measure()
{
    skp1.pause = true;
    skp2.pause = true;

    if (buzzer.IsOn)
        buzzer.close();
}

bool is_tof_pause_all()
{
    if (skp1.pause && skp2.pause)
        return true;

    return false;
}

static void Task1TOF(void *pvParameters)
{
    Serial.printf("%s running on core %d with priority %d\r\n", pcTaskGetTaskName(NULL), xPortGetCoreID(), uxTaskPriorityGet(NULL));

    skp1.start();
    skp2.start();

    for (;;)
    {
        // https://stackoverflow.com/questions/34492501/difference-between-and-or-and
        // https://stackoverflow.com/questions/35301/what-is-the-difference-between-the-and-or-operators
        if (skp1.handler(syscg.alarm.tof1) | skp2.handler(syscg.alarm.tof2))
            buzzer.open();

        vTaskDelay(1); // vTaskDelay() = delay()
    }
}

static void Task2Scan(void *pvParameters)
{
    Serial.printf("%s running on core %d with priority %d\r\n", pcTaskGetTaskName(NULL), xPortGetCoreID(), uxTaskPriorityGet(NULL));

    bms.setup();

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
                        doc["mac"] = syscg.mqtt.macddr;
                        doc["tid"] = tid;
                        doc["distance"] = alog[tid].distance;
                        doc["limit"] = alog[tid].limit;
                        serializeJson(doc, output);
                        mqttClient.publish(syscg.mqtt.publish.c_str(), 0, false, output.c_str());
                        alog[tid].onpub = false;
                    }
                }
            }
        }

        bms.update();
        vTaskDelay(1);
    }
}

static void Task3TFT(void *pvParameters)
{
    Serial.printf("%s running on core %d with priority %d\r\n", pcTaskGetTaskName(NULL), xPortGetCoreID(), uxTaskPriorityGet(NULL));

    set_lvgl();

    for (;;)
    {
        lvtft_task_handler();
        vTaskDelay(1);
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    set_littlefs();

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

    if (conn_sign.web)
    {
        // only exec once.
        conn_sign.web = false;
        if (!conn_wifi(conn_sign.ssid, conn_sign.pwd))
        {
            // connect fail rollback to known wifi.
            conn_wifi(false);
        }
    }

    if (conn_sign.wifi)
    {
        conn_sign.wifi = false;
        if (!WiFi.isConnected())
            conn_wifi(true);
    }

    if (conn_sign.mqtt && WiFi.isConnected())
    {
        conn_sign.mqtt = false;
        mqttClient.connect();
    }
}
