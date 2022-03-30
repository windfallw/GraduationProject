#ifndef LVUIH
#define LVUIH

#include "lv_tft.h"

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

/* encoder */
extern lv_group_t *encoderGroup;

extern void set_lv_mainScreen();
extern void mainScreenAlign();

extern void set_lv_chargeScreen();

extern void set_encoderGroup();

#endif