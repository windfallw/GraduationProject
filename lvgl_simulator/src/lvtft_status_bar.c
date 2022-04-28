#include "lvtft_conf.h"
#include "lvtft_style.h"
#include "lvtft_status_bar.h"

/*
 * @brief set the top status bar on lv_layer_top()
 */
void set_lv_top_status_bar()
{
    top_bar = lv_mem_alloc(sizeof(top_status_bar_t));

    top_bar->obj = lv_obj_create(lv_layer_top());
    lv_obj_clear_flag(top_bar->obj, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(top_bar->obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_style_all(top_bar->obj);
    lv_obj_add_style(top_bar->obj, &style_top_status_bar, 0);

    /* wifi icon */
    top_bar->wifi_ico = lv_label_create(top_bar->obj);
    lv_obj_add_style(top_bar->wifi_ico, &style_top_status_bar_font, 0);
    lv_label_set_text(top_bar->wifi_ico, LV_SYMBOL_WIFI);

    /* wifi text */
    top_bar->wifi_txt = lv_label_create(top_bar->obj);
    lv_obj_add_style(top_bar->wifi_txt, &style_top_status_bar_font, 0);

    /* battery level txt */
    top_bar->level_txt = lv_label_create(top_bar->obj);
    lv_obj_add_style(top_bar->level_txt, &style_top_status_bar_font, 0);

    /* battery level icon */
    top_bar->level_ico = lv_label_create(top_bar->obj);
    lv_obj_add_style(top_bar->level_ico, &style_top_status_bar_font, 0);

    /* battery lightning icon */
    top_bar->lightning_ico = lv_label_create(top_bar->obj);
    lv_obj_add_flag(top_bar->lightning_ico, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_style(top_bar->lightning_ico, &style_top_status_bar_font, 0);
    lv_obj_set_style_text_color(top_bar->lightning_ico, light_color, 0);
    lv_label_set_text(top_bar->lightning_ico, LV_SYMBOL_CHARGE);

    /******************
     *     ALIGN      *
     ******************/
    lv_obj_align(top_bar->wifi_ico, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_align(top_bar->level_txt, LV_ALIGN_RIGHT_MID, 0, 0);

    align_lv_top_status_bar();
}

/*
 * @brief align the top status bar
 */
void align_lv_top_status_bar()
{
    lv_obj_align_to(top_bar->wifi_txt, top_bar->wifi_ico, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_obj_align_to(top_bar->level_ico, top_bar->level_txt, LV_ALIGN_OUT_LEFT_MID, 0, 0);
    lv_obj_align_to(top_bar->lightning_ico, top_bar->level_ico, LV_ALIGN_OUT_LEFT_MID, 0, 0);
}

/*
 * @brief set the bottom status bar on lv_layer_top()
 */
void set_lv_bottom_status_bar()
{
    bottom_bar = lv_mem_alloc(sizeof(bottom_status_bar_t));
    bottom_bar->obj = lv_obj_create(lv_layer_top());
    lv_obj_clear_flag(bottom_bar->obj, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(bottom_bar->obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_style_all(bottom_bar->obj);
    lv_obj_add_style(bottom_bar->obj, &style_bottom_status_bar, 0);

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