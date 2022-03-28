#ifndef LVUIH
#define LVUIH

#include "lv_tft.h"
// #include "charging.c"

extern lv_obj_t *wifi_ico;
extern lv_obj_t *wifi_label;

extern lv_obj_t *battery_label;
extern lv_obj_t *battery_ico;
extern lv_obj_t *battery_cg_ico;

extern lv_obj_t *tof_label;
extern lv_obj_t *bt_label;

extern void set_ui();
extern void ui_align();

#endif