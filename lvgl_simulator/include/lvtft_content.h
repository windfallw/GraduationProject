#ifndef LVTFT_CONTENT_H
#define LVTFT_CONTENT_H

#include "lvgl.h"

/*****************
 *  MAIN SCREEN  *
 *****************/
lv_obj_t *main_screen;
lv_obj_t *main_screen_bg;

/* main screen menu */
lv_obj_t *main_screen_menu;

/*******************
 *  CHARGE SCREEN  *
 *******************/
lv_obj_t *charge_screen;

lv_obj_t *charge_animimg;
lv_obj_t *charge_txt[3];

void set_lv_main_screen();

void set_lv_charge_screen();

#endif