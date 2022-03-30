#ifndef LVTFTH
#define LVTFTH

#include "lvgl.h"

/* Top Status Bar */
extern lv_obj_t *top_status_bar;

extern lv_obj_t *wifi_ico;
extern lv_obj_t *wifi_txt;

extern lv_obj_t *battery_level_txt;
extern lv_obj_t *battery_level_ico;
extern lv_obj_t *battery_lightning_ico;

/* Main Screen */
extern lv_obj_t *main_screen;

/* charge Screen */
extern lv_obj_t *charge_screen;
extern lv_obj_t *charge_animimg;
extern lv_obj_t *charge_txt[3];

extern void set_lvgl();

extern void align_lv_top_status_bar();

#endif