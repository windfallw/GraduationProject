#include "lvtft_conf.h"
#include "lvtft_style.h"
#include "lvtft_status_bar.h"

/*
 * @brief set the top status bar on lv_layer_top()
 */
void set_lv_top_status_bar()
{
    top_status_bar = lv_obj_create(lv_layer_top());
    lv_obj_clear_flag(top_status_bar, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(top_status_bar, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_style_all(top_status_bar);
    lv_obj_add_style(top_status_bar, &style_top_status_bar, 0);

    /* wifi icon */
    wifi_ico = lv_label_create(top_status_bar);
    lv_obj_add_style(wifi_ico, &style_top_status_bar_font, 0);
    lv_label_set_text(wifi_ico, LV_SYMBOL_WIFI);

    /* wifi text */
    wifi_txt = lv_label_create(top_status_bar);
    lv_obj_add_style(wifi_txt, &style_top_status_bar_font, 0);

    /* battery level txt */
    battery_level_txt = lv_label_create(top_status_bar);
    lv_obj_add_style(battery_level_txt, &style_top_status_bar_font, 0);

    /* battery level icon */
    battery_level_ico = lv_label_create(top_status_bar);
    lv_obj_add_style(battery_level_ico, &style_top_status_bar_font, 0);

    /* battery lightning icon */
    battery_lightning_ico = lv_label_create(top_status_bar);
    lv_obj_add_flag(battery_lightning_ico, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_style(battery_lightning_ico, &style_top_status_bar_font, 0);
    lv_obj_set_style_text_color(battery_lightning_ico, light_color, 0);
    lv_label_set_text(battery_lightning_ico, LV_SYMBOL_CHARGE);

    /******************
     *     ALIGN      *
     ******************/
    lv_obj_align(wifi_ico, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_align(battery_level_txt, LV_ALIGN_RIGHT_MID, 0, 0);

    align_lv_top_status_bar();
}

/*
 * @brief align the top status bar
 */
void align_lv_top_status_bar()
{
    lv_obj_align_to(wifi_txt, wifi_ico, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_obj_align_to(battery_level_ico, battery_level_txt, LV_ALIGN_OUT_LEFT_MID, 0, 0);
    lv_obj_align_to(battery_lightning_ico, battery_level_ico, LV_ALIGN_OUT_LEFT_MID, 0, 0);
}

/*
 * @brief set the bottom status bar on lv_layer_top()
 */
void set_lv_bottom_status_bar()
{
    bottom_status_bar = lv_obj_create(lv_layer_top());
    lv_obj_clear_flag(bottom_status_bar, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(bottom_status_bar, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_style_all(bottom_status_bar);
    lv_obj_add_style(bottom_status_bar, &style_bottom_status_bar, 0);

    /******************
     *     ALIGN      *
     ******************/

    align_lv_bottom_status_bar();
}

/*
 * @brief align the bottom status bar
 */
void align_lv_bottom_status_bar()
{
}