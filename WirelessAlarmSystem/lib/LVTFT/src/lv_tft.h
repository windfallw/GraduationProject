#ifndef LVTFTH
#define LVTFTH

#include "AiEsp32RotaryEncoder.h"
#include "TFT_eSPI.h"
#include "lvgl.h"

extern AiEsp32RotaryEncoder rotaryEncoder;

extern TFT_eSPI tft;

extern lv_disp_t *disp;
extern lv_indev_t *indev;

extern void set_rotary_encoder();
extern void set_disp_drv();

#endif