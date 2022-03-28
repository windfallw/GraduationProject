#include "header.h"

void update_ui()
{
    lv_label_set_text_fmt(wifi_label, "STA %s | AP %s", WiFi.SSID().c_str(), WiFi.softAPSSID().c_str());
    lv_label_set_text_fmt(battery_label, "%d%%", bms.percent);

    if (bms.percent >= 80)
        lv_label_set_text(battery_ico, LV_SYMBOL_BATTERY_FULL);
    else if (bms.percent >= 60)
        lv_label_set_text(battery_ico, LV_SYMBOL_BATTERY_3);
    else if (bms.percent >= 40)
        lv_label_set_text(battery_ico, LV_SYMBOL_BATTERY_2);
    else if (bms.percent >= 20)
        lv_label_set_text(battery_ico, LV_SYMBOL_BATTERY_1);
    else
        lv_label_set_text(battery_ico, LV_SYMBOL_BATTERY_EMPTY);

    if (bms.isCharging)
        lv_obj_clear_flag(battery_cg_ico, LV_OBJ_FLAG_HIDDEN);
    else
        lv_obj_add_flag(battery_cg_ico, LV_OBJ_FLAG_HIDDEN);

    lv_label_set_text_fmt(tof_label, "当前: %d mm %d mm", skp1.distance, skp2.distance);
    lv_label_set_text_fmt(bt_label, "%d mA %d mV %d mV %d", bms.current, bms.voltage, bms.voltageOc, bms.State);

    ui_align();
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
