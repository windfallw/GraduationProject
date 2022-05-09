#include "lvtft_drv.hpp"
#include "lvtft_conf.h"
#include "lvtft_style.h"
#include "lvtft_status_bar.h"
#include "lvtft_content.h"

#include "var.hpp"

static char *generate_qrcode_wifi_data()
{
    const char *format = "WIFI:T:WPA2;S:%s;P:%s;H:false;";
    const size_t size = strlen(format) + strlen(syscg.ap.ssid.c_str()) + strlen(syscg.ap.pwd.c_str());
    char *data = (char *)malloc(size);
    sprintf(data, format, syscg.ap.ssid.c_str(), syscg.ap.pwd.c_str());
    return data;
}

static void tof_sw_event_cb(lv_event_t *e)
{
    // lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    lv_obj_has_state(obj, LV_STATE_CHECKED) ? pause_tof_measure() : resume_tof_measure();
}

static void tof_slider_event_cg_cb(lv_event_t *e)
{
    menu_slider_t *obj = (menu_slider_t *)lv_event_get_user_data(e);
    lv_label_set_text_fmt(obj->slider_val, "%d", lv_slider_get_value(obj->slider));
}

static void buzzer_duty_slider_event_cg_cb(lv_event_t *e)
{
    menu_slider_t *obj = (menu_slider_t *)lv_event_get_user_data(e);
    uint32_t value = lv_slider_get_value(obj->slider);
    lv_label_set_text_fmt(obj->val, "%d", value);

    if (is_tof_pause_all())
        buzzer.writeCycle(value);
}

static void buzzer_freq_slider_event_cg_cb(lv_event_t *e)
{
    menu_slider_t *obj = (menu_slider_t *)lv_event_get_user_data(e);
    uint32_t value = lv_slider_get_value(obj->slider);
    lv_label_set_text_fmt(obj->val, "%d", value);

    if (is_tof_pause_all())
        buzzer.writeFreq(value, true);
}

static void buzzer_slider_event_ext_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    uint32_t *value = (uint32_t *)lv_event_get_user_data(e);
    *value = lv_slider_get_value(obj);
    writeConfigFile();

    if (is_tof_pause_all())
        buzzer.writeCycle(0);
}

static void slider_event_cg_cb(lv_event_t *e)
{
    menu_slider_t *obj = (menu_slider_t *)lv_event_get_user_data(e);
    lv_label_set_text_fmt(obj->val, "%d", lv_slider_get_value(obj->slider));
}

static void slider_event_ext_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    uint32_t *value = (uint32_t *)lv_event_get_user_data(e);
    *value = lv_slider_get_value(obj);
    writeConfigFile();
}

static void set_lv_event()
{
    lv_obj_add_event_cb(tof_mute_switch->sw, tof_sw_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_add_event_cb(tof_limit_slider1->slider, tof_slider_event_cg_cb, LV_EVENT_VALUE_CHANGED, tof_limit_slider1);
    lv_obj_add_event_cb(tof_limit_slider1->slider, slider_event_ext_cb, LV_EVENT_RELEASED, &syscg.alarm.tof1);

    lv_obj_add_event_cb(tof_limit_slider2->slider, tof_slider_event_cg_cb, LV_EVENT_VALUE_CHANGED, tof_limit_slider2);
    lv_obj_add_event_cb(tof_limit_slider2->slider, slider_event_ext_cb, LV_EVENT_RELEASED, &syscg.alarm.tof2);

    lv_obj_add_event_cb(buzzer_duty_slider->slider, buzzer_duty_slider_event_cg_cb, LV_EVENT_VALUE_CHANGED, buzzer_duty_slider);
    lv_obj_add_event_cb(buzzer_duty_slider->slider, buzzer_slider_event_ext_cb, LV_EVENT_RELEASED, &syscg.alarm.dutyCycle);

    lv_obj_add_event_cb(buzzer_freq_slider->slider, buzzer_freq_slider_event_cg_cb, LV_EVENT_VALUE_CHANGED, buzzer_freq_slider);
    lv_obj_add_event_cb(buzzer_freq_slider->slider, buzzer_slider_event_ext_cb, LV_EVENT_RELEASED, &syscg.alarm.freq);

    lv_obj_add_event_cb(buzzer_ms_slider->slider, slider_event_cg_cb, LV_EVENT_VALUE_CHANGED, buzzer_ms_slider);
    lv_obj_add_event_cb(buzzer_ms_slider->slider, slider_event_ext_cb, LV_EVENT_RELEASED, &syscg.alarm.ms);
}

static void set_lv_content_sub()
{
    /* subpage network content */
    nw_sta_ip = create_menu_text(menu_sub_nw->section, "Station IP", NULL);
    nw_ap = create_menu_text(menu_sub_nw->section, "Access Point", syscg.ap.ssid.c_str());
    nw_ap_qrcode = create_menu_qrcode(menu_sub_nw->section, NULL, "AP QRCode", generate_qrcode_wifi_data());

    /* subpage tof content */
    tof_mute_switch = create_menu_switch(menu_sub_tof->section, LV_SYMBOL_WARNING, "Pause Alert", DEFAULT_PAUSE);
    tof_limit_slider1 = create_menu_slider(menu_sub_tof->section, LV_SYMBOL_SETTINGS, "tof lidar 1 (mm)", syscg.alarm.tofMin, syscg.alarm.tofMax, syscg.alarm.tof1, false);
    tof_limit_slider2 = create_menu_slider(menu_sub_tof->section, LV_SYMBOL_SETTINGS, "tof lidar 2 (mm)", syscg.alarm.tofMin, syscg.alarm.tofMax, syscg.alarm.tof2, false);

    /* subpage buzzer content */
    buzzer_duty_slider = create_menu_slider(menu_sub_buzzer->section, LV_SYMBOL_MUTE, "duty cycle", 0, MAXDUTYCYCLE, syscg.alarm.dutyCycle, true);
    buzzer_freq_slider = create_menu_slider(menu_sub_buzzer->section, LV_SYMBOL_AUDIO, "frequency", 0, MAXFREQ, syscg.alarm.freq, true);
    buzzer_ms_slider = create_menu_slider(menu_sub_buzzer->section, LV_SYMBOL_LOOP, "close delay (ms)", syscg.alarm.msMin, syscg.alarm.msMax, syscg.alarm.ms, true);

    /* subpage bms content */
    bms_current = create_menu_text(menu_sub_bms->section, "Current (mA)", NULL);
    bms_voltage = create_menu_text(menu_sub_bms->section, "Voltage (mV)", NULL);
    bms_voltage_oc = create_menu_text(menu_sub_bms->section, "VoltageOC (mV)", NULL);
    bms_state = create_menu_text(menu_sub_bms->section, "State Code", NULL);
}

static void set_lv_indev_group()
{
    set_indev_group_obj(enter_tof_page->menu_cont);
    set_indev_group_obj(enter_nw_page->menu_cont);
    set_indev_group_obj(enter_buzzer_page->menu_cont);
    set_indev_group_obj(enter_bms_page->menu_cont);
}

void set_lvgl()
{
    set_lv_drv();
    set_lv_style();

    set_lv_top_status_bar();
    set_lv_bottom_status_bar();
    set_lv_content_main();
    set_lv_content_sub();

    set_lv_event();
    set_lv_indev_group();

    menu_load_page(enter_nw_page);
    lv_scr_load(main_screen);
}

void lvtft_task_handler()
{
    // lv_obj_t *cur_page = lv_menu_get_cur_main_page(main_screen_menu);

    lv_label_set_text(top_bar->wifi_txt, WiFi.SSID().c_str());
    lv_label_set_text_fmt(top_bar->level_txt, "%d%%", bms.percent);

    lv_label_set_text(nw_sta_ip->content, WiFi.localIP().toString().c_str());

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