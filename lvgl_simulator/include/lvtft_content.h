#ifndef LVTFT_CONTENT_H
#define LVTFT_CONTENT_H

#include "lvgl.h"

lv_obj_t *main_screen;
lv_obj_t *main_screen_bg;
lv_obj_t *main_screen_menu;

lv_obj_t *menu_root_page;
lv_obj_t *root_page_section;

/* tof sub page */
lv_obj_t *tof_page_enter;

lv_obj_t *tof_page;
lv_obj_t *tof_page_section;

lv_obj_t *tof_limit_slider1;
lv_obj_t *tof_limit_slider2;

/* buzzer sub page */
lv_obj_t *buzzer_page_enter;

lv_obj_t *buzzer_page;
lv_obj_t *buzzer_page_section;

lv_obj_t *buzzer_mute_switch;
lv_obj_t *buzzer_duty_slider;
lv_obj_t *buzzer_freq_slider;

/* charge */
lv_obj_t *charge_screen;
lv_obj_t *charge_animimg;
lv_obj_t *charge_txt[3];

void set_lv_main_screen();

void set_lv_charge_screen();

#endif