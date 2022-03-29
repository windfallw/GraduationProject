#ifndef LVUIH
#define LVUIH

#include "lv_tft.h"

extern lv_group_t *encoderGroup;

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

/* test */
extern lv_obj_t *tof_label;
extern lv_obj_t *bt_label;

extern void set_ui();
extern void mainScreenAlign();

#endif