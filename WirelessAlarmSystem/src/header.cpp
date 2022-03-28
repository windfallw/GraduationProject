#include "header.h"

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