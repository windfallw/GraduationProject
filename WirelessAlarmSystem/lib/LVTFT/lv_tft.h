#ifndef LVTFTH
#define LVTFTH

#include "AiEsp32RotaryEncoder.h"
#include "TFT_eSPI.h"

#include "lvgl.h"

#include "file.h"

extern AiEsp32RotaryEncoder rotaryEncoder;

extern TFT_eSPI tft;

extern lv_obj_t *wifi_ico;
extern lv_obj_t *wifi_label;

extern lv_obj_t *battery_label;
extern lv_obj_t *battery_ico;
extern lv_obj_t *battery_cg_ico;

extern lv_obj_t *tof_label;
extern lv_obj_t *bt_label;

extern void set_rotary_encoder();
extern void set_disp_drv();
extern void set_ui();
extern void lv_obj_align_all();


#endif