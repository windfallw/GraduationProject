#include "lvtft_conf.h"
#include "lvtft_style.h"
#include "lvtft.h"

/*
 * @brief set lvgl and load main screen
 */
void set_lvgl()
{
    set_lv_style();

    set_lv_top_status_bar();

    set_lv_bottom_status_bar();

    set_lv_main_screen();
}