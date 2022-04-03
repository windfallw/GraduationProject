#ifndef LVTFT_STYLE_H
#define LVTFT_STYLE_H

#include "lvgl.h"

/* Color */
lv_color_t light_color;
lv_color_t white_color;
lv_color_t dark_color;
lv_color_t obj_bg_color;

/* Style */
lv_style_t style_top_status_bar;
lv_style_t style_top_status_bar_font;

lv_style_t style_bottom_status_bar;
lv_style_t style_bottom_status_bar_font;

lv_style_t style_main_screen_bg;
lv_style_t style_main_screen_font;

void set_lv_style();

#endif