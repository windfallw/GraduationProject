#ifndef LVTFT_STATUS_BAR_H
#define LVTFT_STATUS_BAR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"

    /* Top Status Bar */
    lv_obj_t *top_status_bar;

    lv_obj_t *wifi_ico;
    lv_obj_t *wifi_txt;

    lv_obj_t *battery_level_txt;
    lv_obj_t *battery_level_ico;
    lv_obj_t *battery_lightning_ico;

    /* Bottom Status Bar */
    lv_obj_t *bottom_status_bar;

    void set_lv_top_status_bar();
    void align_lv_top_status_bar();

    void set_lv_bottom_status_bar();
    void align_lv_bottom_status_bar();

#ifdef __cplusplus
}
#endif

#endif