#ifndef LVTFTH
#define LVTFTH

#include "AiEsp32RotaryEncoder.h"
#include <lvgl.h>
#include <TFT_eSPI.h>

extern AiEsp32RotaryEncoder rotaryEncoder;

extern TFT_eSPI tft;

extern void set_rotary_encoder();
extern void set_disp_drv();

#endif