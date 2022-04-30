#ifndef LVTFT_CONTENT_H
#define LVTFT_CONTENT_H

#ifdef __cplusplus
extern "C"
{
#endif

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
        lv_obj_t *val;
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
    extern lv_obj_t *main_screen;
    extern lv_obj_t *main_screen_bg;
    extern lv_obj_t *main_screen_menu;

    extern lv_obj_t *charge_bg;
    extern lv_obj_t *charge_animimg;
    extern lv_timer_t *charge_anim_timer;

    /* menu root page */
    extern menu_page_t *menu_root;

    /* network subpage */
    extern menu_page_t *menu_sub_nw;
    extern menu_base_t *enter_nw_page;
    extern menu_text_t *nw_sta_ip;
    extern menu_switch_t *nw_ap_switch;
    extern menu_qrcode_t *nw_ap_qrcode;

    /* tof subpage */
    extern menu_page_t *menu_sub_tof;
    extern menu_base_t *enter_tof_page;
    extern menu_slider_t *tof_limit_slider1;
    extern menu_slider_t *tof_limit_slider2;

    /* buzzer subpage */
    extern menu_page_t *menu_sub_buzzer;
    extern menu_base_t *enter_buzzer_page;
    extern menu_switch_t *buzzer_mute_switch;
    extern menu_slider_t *buzzer_duty_slider;
    extern menu_slider_t *buzzer_freq_slider;

    /* bms subpage */
    extern menu_page_t *menu_sub_bms;
    extern menu_base_t *enter_bms_page;
    extern menu_text_t *bms_current;
    extern menu_text_t *bms_voltage;
    extern menu_text_t *bms_voltage_oc;
    extern menu_text_t *bms_state;

    void set_lv_main_screen();
    void show_lv_charge_screen();
    void menu_load_page(menu_base_t *obj);

#ifdef __cplusplus
}
#endif

#endif