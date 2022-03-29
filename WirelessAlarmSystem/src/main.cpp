#include "header.h"

void setup()
{
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    set_littlefs();

    skp1.start();
    skp2.start();

    bms.set_up();

    set_rotary_encoder();
    set_lv_drv();
    set_ui();

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
