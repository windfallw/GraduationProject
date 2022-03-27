#include "lvtft.h"

lv_obj_t *wifi_icon;
lv_obj_t *wifi_label;

lv_obj_t *battery_icon;
lv_obj_t *battery_label;

void set_ui()
{
    wifi_icon = lv_label_create(lv_scr_act());
    lv_obj_align(wifi_icon, LV_ALIGN_TOP_LEFT, 5, 5);
    lv_obj_set_style_text_font(wifi_icon, &lv_font_montserrat_14, 0);
    lv_label_set_text(wifi_icon, LV_SYMBOL_WIFI);

    wifi_label = lv_label_create(lv_scr_act());
    lv_obj_align_to(wifi_label, wifi_icon, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_style_text_font(wifi_label, &lv_font_montserrat_12, 0);
    lv_label_set_text(wifi_label, "WiFi");

    battery_icon = lv_label_create(lv_scr_act());
    lv_obj_align(battery_icon, LV_ALIGN_TOP_RIGHT, -5, 5);
    lv_obj_set_style_text_font(battery_icon, &lv_font_montserrat_14, 0);
    lv_label_set_recolor(battery_icon, true);
    lv_label_set_text_fmt(battery_icon, "#00FF00 %s# %s", LV_SYMBOL_CHARGE, LV_SYMBOL_BATTERY_FULL);
    // lv_label_set_text(battery_icon, LV_SYMBOL_BATTERY_FULL);

    battery_label = lv_label_create(lv_scr_act());
    lv_obj_align_to(battery_label, battery_icon, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_obj_set_style_text_font(battery_label, &lv_font_montserrat_12, 0);
    lv_label_set_text(battery_label, "100%");
}
