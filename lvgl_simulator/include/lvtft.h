#ifndef LVTFTH
#define LVTFTH

#include "lvgl.h"

/* mainScreen */
extern lv_obj_t *mainScreen;

extern lv_obj_t *wifi_ico;
extern lv_obj_t *wifi_label;

extern lv_obj_t *battery_label;
extern lv_obj_t *battery_ico;
extern lv_obj_t *battery_cg_ico;

/* chargeScreen */
extern lv_obj_t *chargeScreen;
extern lv_obj_t *chargeAnimimg;

extern void set_ui();
extern void mainScreenAlign();

#endif