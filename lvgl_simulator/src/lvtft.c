#include "lvtft.h"

lv_obj_t *wifi_ico;
lv_obj_t *wifi_label;

lv_obj_t *battery_label;
lv_obj_t *battery_ico;
lv_obj_t *battery_cg_ico;

void set_ui()
{
    wifi_ico = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(wifi_ico, &lv_font_montserrat_12, 0);
    lv_label_set_text(wifi_ico, LV_SYMBOL_WIFI);

    wifi_label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(wifi_label, &lv_font_montserrat_12, 0);

    battery_label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(battery_label, &lv_font_montserrat_12, 0);

    battery_ico = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(battery_ico, &lv_font_montserrat_12, 0);

    battery_cg_ico = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(battery_cg_ico, &lv_font_montserrat_12, 0);
    lv_label_set_recolor(battery_cg_ico, true);
    lv_label_set_text(battery_cg_ico, "#00FF00 " LV_SYMBOL_CHARGE);
    lv_obj_add_flag(battery_cg_ico, LV_OBJ_FLAG_HIDDEN);
}

void lv_obj_align_all()
{
    lv_obj_align(wifi_ico, LV_ALIGN_TOP_LEFT, 5, 5);
    lv_obj_align_to(wifi_label, wifi_ico, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_obj_align(battery_label, LV_ALIGN_TOP_RIGHT, -5, 5);
    lv_obj_align_to(battery_ico, battery_label, LV_ALIGN_OUT_LEFT_MID, 0, 0);
    lv_obj_align_to(battery_cg_ico, battery_ico, LV_ALIGN_OUT_LEFT_MID, 0, 0);
}
