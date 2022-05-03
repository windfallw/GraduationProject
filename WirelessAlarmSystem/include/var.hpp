#ifndef VAR_H
#define VAR_H

#include "file.h"
#include "tof.h"
#include "IP5108.h"
#include "netsrv.h"
#include "lvtft.hpp"

extern TaskHandle_t Task1;
extern TaskHandle_t Task2;
extern TaskHandle_t Task3;

extern SKPTOFLIDAR skp1;
extern SKPTOFLIDAR skp2;
extern IP5108 bms;
extern shinelight buzzer;

#endif