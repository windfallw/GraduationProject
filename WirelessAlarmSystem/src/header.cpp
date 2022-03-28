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
