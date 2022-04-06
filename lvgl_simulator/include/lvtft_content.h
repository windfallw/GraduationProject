#ifndef LVTFT_CONTENT_H
#define LVTFT_CONTENT_H

#include "lvgl.h"

typedef struct
{
    lv_obj_t *menu_cont;
    lv_obj_t *icon;
    lv_obj_t *title;
} menu_base_t;

typedef struct
{
    menu_base_t *base;
    lv_obj_t *slider;
    lv_obj_t *slider_txt;
} menu_slider_t;

typedef struct
{
    menu_base_t *base;
    lv_obj_t *sw;
} menu_switch_t;

/*********************
 *    main screen    *
 *********************/
lv_obj_t *main_screen;
lv_obj_t *main_screen_bg;
lv_obj_t *main_screen_menu;

/* menu root page */
lv_obj_t *menu_root_page;
lv_obj_t *root_page_section;

/* tof subpage */
lv_obj_t *tof_page;
lv_obj_t *tof_page_section;

menu_base_t *tof_page_enter;
menu_slider_t *tof_limit_slider1;
menu_slider_t *tof_limit_slider2;

/* buzzer subpage */
lv_obj_t *buzzer_page;
lv_obj_t *buzzer_page_section;

menu_base_t *buzzer_page_enter;
menu_switch_t *buzzer_mute_switch;
menu_slider_t *buzzer_duty_slider;
menu_slider_t *buzzer_freq_slider;

/*********************
 *   charge screen   *
 *********************/
lv_obj_t *charge_screen;
lv_obj_t *charge_animimg;

void set_lv_main_screen();
void set_lv_charge_screen();
void show_lv_charge_screen();

#endif