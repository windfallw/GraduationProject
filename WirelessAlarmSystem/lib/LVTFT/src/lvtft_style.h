#ifndef LVTFT_STYLE_H
#define LVTFT_STYLE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"

    /* Color */
    extern lv_color_t light_color;
    extern lv_color_t white_color;
    extern lv_color_t dark_color;
    extern lv_color_t obj_bg_color;

    /* Style */
    extern lv_style_t style_top_status_bar;
    extern lv_style_t style_top_status_bar_font;

    extern lv_style_t style_bottom_status_bar;
    extern lv_style_t style_bottom_status_bar_font;

    extern lv_style_t style_main_screen_bg;
    extern lv_style_t style_main_screen_menu;

    void set_lv_style();

#ifdef __cplusplus
}
#endif

#endif