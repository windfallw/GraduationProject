#ifndef LVTFT_CONTENT_H
#define LVTFT_CONTENT_H

#include "lvgl.h"

typedef struct
{
    lv_obj_t *page;
    lv_obj_t *section;
} menu_page_t;

typedef struct
{
    lv_obj_t *menu_cont;
    lv_obj_t *icon;
    lv_obj_t *title;
} menu_base_t;

typedef struct
{
    lv_obj_t *menu_cont;
    lv_obj_t *title;
    lv_obj_t *content;
} menu_text_t;

typedef struct
{
    menu_base_t *base;
    lv_obj_t *slider;
    lv_obj_t *slider_val;
} menu_slider_t;

typedef struct
{
    menu_base_t *base;
    lv_obj_t *sw;
} menu_switch_t;

typedef struct
{
    menu_base_t *base;
    lv_obj_t *qr;
} menu_qrcode_t;

/*********************
 *    main screen    *
 *********************/
lv_obj_t *main_screen;
lv_obj_t *main_screen_bg;
lv_obj_t *main_screen_menu;

lv_obj_t *charge_bg;
lv_obj_t *charge_animimg;
lv_timer_t *charge_anim_timer;

/* menu root page */
menu_page_t *menu_root;

/* network subpage */
menu_page_t *menu_sub_nw;
menu_base_t *enter_nw_page;
menu_switch_t *nw_ap_switch;
menu_qrcode_t *nw_ap_qrcode;

/* tof subpage */
menu_page_t *menu_sub_tof;
menu_base_t *enter_tof_page;
menu_slider_t *tof_limit_slider1;
menu_slider_t *tof_limit_slider2;

/* buzzer subpage */
menu_page_t *menu_sub_buzzer;
menu_base_t *enter_buzzer_page;
menu_switch_t *buzzer_mute_switch;
menu_slider_t *buzzer_duty_slider;
menu_slider_t *buzzer_freq_slider;

/* bms subpage */
menu_page_t *menu_sub_bms;
menu_base_t *enter_bms_page;
menu_text_t *bms_current;
menu_text_t *bms_voltage;
menu_text_t *bms_voltage_oc;
menu_text_t *bms_state;

void set_lv_main_screen();
void show_lv_charge_anim();
void menu_load_page(menu_base_t *obj);

#endif