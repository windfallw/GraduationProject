#include "lvtft_drv.hpp"
#include "lvtft_conf.h"
#include "lvtft_style.h"
#include "lvtft_status_bar.h"
#include "lvtft_content.h"

#include "var.hpp"

static void set_lv_indev_group()
{
    set_indev_group_obj(enter_tof_page->menu_cont);
    set_indev_group_obj(enter_nw_page->menu_cont);
    set_indev_group_obj(enter_buzzer_page->menu_cont);
    set_indev_group_obj(enter_bms_page->menu_cont);
}

static void switch_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        if (lv_obj_has_state(obj, LV_STATE_CHECKED))
        {
            skp1.pause = true;
            skp2.pause = true;
        }
        else
        {
            skp1.pause = false;
            skp2.pause = false;
        }
    }
}

static void set_lv_event()
{
    lv_obj_add_event_cb(tof_mute_switch->sw, switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

void set_lvgl()
{
    set_lv_drv();

    set_lv_style();

    set_lv_top_status_bar();

    set_lv_bottom_status_bar();

    set_lv_main_screen();

    set_lv_event();

    set_lv_indev_group();

    lv_scr_load(main_screen);
}

void lvtft_handler()
{
    // lv_obj_t *cur_page = lv_menu_get_cur_main_page(main_screen_menu);
    lv_label_set_text(top_bar->wifi_txt, WiFi.SSID().c_str());
    lv_label_set_text_fmt(top_bar->level_txt, "%d%%", bms.percent);

    lv_label_set_text(nw_sta_ip->content, WiFi.localIP().toString().c_str());
    lv_label_set_text(nw_ap->content, WiFi.softAPSSID().c_str());

    if (bms.percent >= 80)
        lv_label_set_text(top_bar->level_ico, LV_SYMBOL_BATTERY_FULL);
    else if (bms.percent >= 60)
        lv_label_set_text(top_bar->level_ico, LV_SYMBOL_BATTERY_3);
    else if (bms.percent >= 40)
        lv_label_set_text(top_bar->level_ico, LV_SYMBOL_BATTERY_2);
    else if (bms.percent >= 20)
        lv_label_set_text(top_bar->level_ico, LV_SYMBOL_BATTERY_1);
    else
        lv_label_set_text(top_bar->level_ico, LV_SYMBOL_BATTERY_EMPTY);

    if (bms.isCharging)
    {
        lv_obj_clear_flag(top_bar->lightning_ico, LV_OBJ_FLAG_HIDDEN);
    }
    else
        lv_obj_add_flag(top_bar->lightning_ico, LV_OBJ_FLAG_HIDDEN);

    lv_label_set_text_fmt(tof_limit_slider1->val, "%d", skp1.distance);
    lv_label_set_text_fmt(tof_limit_slider2->val, "%d", skp2.distance);

    lv_label_set_text_fmt(bms_current->content, "%d", bms.current);
    lv_label_set_text_fmt(bms_voltage->content, "%d", bms.voltage);
    lv_label_set_text_fmt(bms_voltage_oc->content, "%d", bms.voltageOc);
    lv_label_set_text_fmt(bms_state->content, "%d", bms.State);

    align_lv_top_status_bar();
    align_lv_bottom_status_bar();

    lv_task_handler();
}