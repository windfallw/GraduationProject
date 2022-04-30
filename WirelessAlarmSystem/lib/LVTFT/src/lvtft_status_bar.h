#ifndef LVTFT_STATUS_BAR_H
#define LVTFT_STATUS_BAR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"

    /* Top Status Bar */
    typedef struct
    {
        lv_obj_t *obj;
        lv_obj_t *wifi_ico;
        lv_obj_t *wifi_txt;
        lv_obj_t *level_txt;
        lv_obj_t *level_ico;
        lv_obj_t *lightning_ico;
    } top_status_bar_t;

    extern top_status_bar_t *top_bar;

    /* Bottom Status Bar */
    typedef struct
    {
        lv_obj_t *obj;
    } bottom_status_bar_t;

    extern bottom_status_bar_t *bottom_bar;

    void set_lv_top_status_bar();
    void align_lv_top_status_bar();

    void set_lv_bottom_status_bar();
    void align_lv_bottom_status_bar();

#ifdef __cplusplus
}
#endif

#endif