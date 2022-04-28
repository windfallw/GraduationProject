#include "lvtft_conf.h"
#include "lvtft_style.h"
#include "lvtft_content.h"

LV_IMG_DECLARE(charging1);
LV_IMG_DECLARE(charging2);
LV_IMG_DECLARE(charging3);
LV_IMG_DECLARE(charging4);
LV_IMG_DECLARE(charging5);
LV_IMG_DECLARE(charging6);
LV_IMG_DECLARE(charging7);
LV_IMG_DECLARE(charging8);
LV_IMG_DECLARE(charging9);
LV_IMG_DECLARE(charging10);
LV_IMG_DECLARE(charging11);
LV_IMG_DECLARE(charging12);

static const lv_img_dsc_t *chargeImgs[12] = {
    &charging1,
    &charging2,
    &charging3,
    &charging4,
    &charging5,
    &charging6,
    &charging7,
    &charging8,
    &charging9,
    &charging10,
    &charging11,
    &charging12,
};

static menu_base_t *create_menu_base(lv_obj_t *parent, const char *icon, const char *title)
{
    menu_base_t *base = lv_mem_alloc(sizeof(menu_base_t));

    base->menu_cont = lv_menu_cont_create(parent);

    if (icon)
    {
        base->icon = lv_img_create(base->menu_cont);
        lv_img_set_src(base->icon, icon);
    }
    else
        base->icon = NULL;

    if (title)
    {
        base->title = lv_label_create(base->menu_cont);
        lv_label_set_text(base->title, title);
        lv_label_set_long_mode(base->title, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_flex_grow(base->title, 1);
    }
    else
        base->title = NULL;

    return base;
}

static menu_slider_t *create_menu_slider(lv_obj_t *parent, const char *icon, const char *title, uint32_t min, uint32_t max, uint32_t val)
{
    menu_slider_t *obj = lv_mem_alloc(sizeof(menu_slider_t));

    obj->base = create_menu_base(parent, icon, title);
    lv_obj_set_style_pad_hor(obj->base->menu_cont, MAIN_SCREEN_MENU_PAGE_CONT_PAD_HOR, 0);

    obj->slider_val = lv_label_create(obj->base->menu_cont);
    lv_label_set_long_mode(obj->slider_val, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_flex_grow(obj->slider_val, 0);

    lv_label_set_text_fmt(obj->slider_val, "%d", val);

    obj->slider = lv_slider_create(obj->base->menu_cont);
    lv_obj_set_style_radius(obj->slider, LV_RADIUS_CIRCLE, LV_PART_KNOB);
    lv_obj_set_style_pad_all(obj->slider, 2, LV_PART_KNOB);
    lv_obj_set_flex_grow(obj->slider, 1);
    lv_obj_add_flag(obj->slider, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    /* set the slider to react on dragging the knob only */
    lv_obj_add_flag(obj->slider, LV_OBJ_FLAG_ADV_HITTEST);

    lv_slider_set_mode(obj->slider, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_range(obj->slider, min, max);
    lv_slider_set_value(obj->slider, val, LV_ANIM_OFF);

    return obj;
}

static menu_switch_t *create_menu_switch(lv_obj_t *parent, const char *icon, const char *title, bool check)
{
    menu_switch_t *obj = lv_mem_alloc(sizeof(menu_switch_t));

    obj->base = create_menu_base(parent, icon, title);

    obj->sw = lv_switch_create(obj->base->menu_cont);

    lv_obj_add_state(obj->sw, check ? LV_STATE_CHECKED : 0);

    lv_obj_set_style_pad_hor(obj->base->menu_cont, MAIN_SCREEN_MENU_PAGE_CONT_PAD_HOR, 0);

    return obj;
}

static menu_qrcode_t *create_menu_qrcode(lv_obj_t *parent, const char *icon, const char *title)
{
    menu_qrcode_t *obj = lv_mem_alloc(sizeof(menu_qrcode_t));

    obj->base = create_menu_base(parent, icon, title);

    obj->qr = lv_qrcode_create(obj->base->menu_cont, MAIN_SCREEN_MENU_QR_CODE_SIZE, obj_bg_color, white_color);

    const char *data = "WIFI:T:WPA2;S:ESP2BF8;P:12345678;H:false;";

    lv_qrcode_update(obj->qr, data, strlen(data));
    lv_obj_center(obj->qr);

    lv_obj_set_style_border_color(obj->qr, white_color, 0);
    lv_obj_set_style_border_width(obj->qr, MAIN_SCREEN_MENU_QR_CODE_BORDER_WIDTH, 0);

    lv_obj_set_style_pad_hor(obj->base->menu_cont, MAIN_SCREEN_MENU_PAGE_CONT_PAD_HOR, 0);

    return obj;
}

static menu_page_t *create_menu_subpage(lv_obj_t *parent, char *title)
{
    menu_page_t *obj = lv_mem_alloc(sizeof(menu_page_t));

    obj->page = lv_menu_page_create(parent, title);
    lv_menu_separator_create(obj->page);
    lv_obj_set_style_pad_hor(obj->page, MAIN_SCREEN_MENU_SUBPAGE_PAD_HOR, 0);

    obj->section = lv_menu_section_create(obj->page);

    return obj;
}

/*
 * @brief handler for the root back button
 * @param event lv_event_t event
 */
static void root_back_btn_handler(lv_event_t *event)
{
    lv_obj_t *obj = lv_event_get_target(event);
    lv_obj_t *menu = lv_event_get_user_data(event);

    if (lv_menu_back_btn_is_root(menu, obj))
    {
        if (lv_menu_get_cur_sidebar_page(menu) == menu_root->page)
        {
            /* remove sidebar page */
            lv_menu_set_sidebar_page(menu, NULL);
            lv_menu_clear_history(menu);
            lv_menu_set_page(menu, menu_root->page);
        }
        else
        {
            /* show sidebar page */
            lv_menu_set_page(menu, NULL);
            lv_menu_set_sidebar_page(menu, menu_root->page);

            /* ! set sidebar header icon after sidebar page is set ! */
            // lv_img_set_src(lv_obj_get_child(lv_menu_get_sidebar_header_back_btn(menu), 0), LV_SYMBOL_REFRESH);
            menu_load_page(enter_nw_page);
        }
    }
}

/*
 * @brief set the main screen menu
 * @param parent lv_obj_t *parent object
 */
static void set_lv_main_screen_menu(lv_obj_t *parent)
{
    /* create main screen menu */
    main_screen_menu = lv_menu_create(parent);
    lv_obj_clear_flag(main_screen_menu, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(main_screen_menu, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(main_screen_menu, &style_main_screen_menu, 0);

    lv_img_set_src(lv_obj_get_child(lv_menu_get_main_header_back_btn(main_screen_menu), 0), LV_SYMBOL_RIGHT);
    lv_menu_set_mode_root_back_btn(main_screen_menu, LV_MENU_ROOT_BACK_BTN_ENABLED);
    lv_obj_add_event_cb(main_screen_menu, root_back_btn_handler, LV_EVENT_CLICKED, main_screen_menu);

    /* create subpage network */
    menu_sub_nw = create_menu_subpage(main_screen_menu, "Network Settings");
    nw_ap_switch = create_menu_switch(menu_sub_nw->section, LV_SYMBOL_WIFI, "Access Point", false);
    nw_ap_qrcode = create_menu_qrcode(menu_sub_nw->section, NULL, "AP QRCode");

    /* create subpage tof */
    menu_sub_tof = create_menu_subpage(main_screen_menu, "Alarm Threshold");
    tof_limit_slider1 = create_menu_slider(menu_sub_tof->section, LV_SYMBOL_SETTINGS, "tof lidar 1", 0, 20000, 1000);
    tof_limit_slider2 = create_menu_slider(menu_sub_tof->section, LV_SYMBOL_SETTINGS, "tof lidar 2", 0, 20000, 2333);

    /* create subpage buzzer */
    menu_sub_buzzer = create_menu_subpage(main_screen_menu, "PWM of Buzzer & LED");
    buzzer_mute_switch = create_menu_switch(menu_sub_buzzer->section, LV_SYMBOL_AUDIO, "mute", false);
    buzzer_duty_slider = create_menu_slider(menu_sub_buzzer->section, NULL, "duty cycle", 0, 255, 30);
    buzzer_freq_slider = create_menu_slider(menu_sub_buzzer->section, NULL, "frequency", 0, 100, 10);

    /* create subpage bms */
    menu_sub_bms = create_menu_subpage(main_screen_menu, "Battery Information");
    bms_current = create_menu_base(menu_sub_bms->section, "Current (mA)", "1000");
    bms_voltage = create_menu_base(menu_sub_bms->section, "Voltage (mV)", "12.3");
    bms_voltage_oc = create_menu_base(menu_sub_bms->section, "VoltageOC (mV)", "12.3");
    bms_state = create_menu_base(menu_sub_bms->section, "State Code", "92");

    /* create root page */
    menu_root = lv_mem_alloc(sizeof(menu_page_t));
    menu_root->page = lv_menu_page_create(main_screen_menu, "Sidebar");
    lv_obj_set_style_pad_all(menu_root->page, MAIN_SCREEN_MENU_ROOTPAGE_PAD_ALL, 0);

    menu_root->section = lv_menu_section_create(menu_root->page);

    enter_nw_page = create_menu_base(menu_root->section, LV_SYMBOL_SETTINGS, "Service");
    enter_tof_page = create_menu_base(menu_root->section, LV_SYMBOL_SETTINGS, "Measure");
    enter_buzzer_page = create_menu_base(menu_root->section, LV_SYMBOL_SETTINGS, "Alarm");
    enter_bms_page = create_menu_base(menu_root->section, LV_SYMBOL_SETTINGS, "Battery");

    /* set the load page event */
    lv_menu_set_load_page_event(main_screen_menu, enter_nw_page->menu_cont, menu_sub_nw->page);
    lv_menu_set_load_page_event(main_screen_menu, enter_tof_page->menu_cont, menu_sub_tof->page);
    lv_menu_set_load_page_event(main_screen_menu, enter_buzzer_page->menu_cont, menu_sub_buzzer->page);
    lv_menu_set_load_page_event(main_screen_menu, enter_bms_page->menu_cont, menu_sub_bms->page);

    /* set siderbar page */
    lv_menu_set_sidebar_page(main_screen_menu, menu_root->page);

    /* ! set sidebar header icon after sidebar page is set ! */
    // lv_img_set_src(lv_obj_get_child(lv_menu_get_sidebar_header_back_btn(main_screen_menu), 0), LV_SYMBOL_REFRESH);
    menu_load_page(enter_nw_page);
}

/*
 * @brief set the main screen
 */
void set_lv_main_screen()
{
    main_screen = lv_obj_create(NULL);

    main_screen_bg = lv_obj_create(main_screen);
    lv_obj_clear_flag(main_screen_bg, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(main_screen_bg, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_style_all(main_screen_bg);
    lv_obj_add_style(main_screen_bg, &style_main_screen_bg, 0);

    /* create menu on main screen background */
    set_lv_main_screen_menu(main_screen_bg);
}

void menu_load_page(menu_base_t *obj)
{
    lv_event_send(obj->menu_cont, LV_EVENT_CLICKED, NULL);
}

/*
 * @brief set the charge screen
 */
void set_lv_charge_screen()
{
    charge_screen = lv_obj_create(NULL);
    lv_obj_clear_flag(main_screen_bg, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(main_screen_bg, LV_OBJ_FLAG_SCROLLABLE);

    charge_animimg = lv_animimg_create(charge_screen);
    lv_obj_align(charge_animimg, LV_ALIGN_CENTER, 0, 0);
    lv_animimg_set_src(charge_animimg, (lv_img_dsc_t **)chargeImgs, 12);
    lv_animimg_set_duration(charge_animimg, 1000);
    lv_animimg_set_repeat_count(charge_animimg, 1);
}

/*
 * @brief show charging animation and return to main screen
 */
void show_lv_charge_screen()
{
    lv_animimg_start(charge_animimg);
    lv_scr_load(charge_screen);
    lv_scr_load_anim(main_screen, LV_SCR_LOAD_ANIM_FADE_ON, 1000, 0, false);
}
