#ifndef HEADERH
#define HEADERH

#include "file.h"
#include "tof.h"
#include "IP5108.h"
#include "lv_ui.h"
#include "netsrv.h"

extern TaskHandle_t Task1;
extern TaskHandle_t Task2;
extern TaskHandle_t Task3;

extern SKPTOFLIDAR skp1;
extern SKPTOFLIDAR skp2;

extern IP5108 bms;

extern shinelight buzzer;

extern void update_ui();

extern void Task1code(void *pvParameters);
extern void Task2code(void *pvParameters);
extern void Task3code(void *pvParameters);

#endif