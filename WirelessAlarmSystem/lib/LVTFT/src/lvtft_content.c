#include "lvtft_conf.h"
#include "lvtft_style.h"
#include "lvtft_content.h"

/*********************
 *    main screen    *
 *********************/
lv_obj_t *main_screen;
lv_obj_t *main_screen_bg;
lv_obj_t *main_screen_menu;

lv_obj_t *charge_bg;
lv_obj_t *charge_animimg;
lv_timer_t *charge_anim_timer;

/* menu root page */
menu_page_t *menu_root;

/* network subpage */
menu_page_t *menu_sub_nw;
menu_base_t *enter_nw_page;
menu_text_t *nw_sta_ip;
menu_text_t *nw_ap;
menu_qrcode_t *nw_ap_qrcode;

/* tof subpage */
menu_page_t *menu_sub_tof;
menu_base_t *enter_tof_page;
menu_switch_t *tof_mute_switch;
menu_slider_t *tof_limit_slider1;
menu_slider_t *tof_limit_slider2;

/* buzzer subpage */
menu_page_t *menu_sub_buzzer;
menu_base_t *enter_buzzer_page;
menu_slider_t *buzzer_ms_slider;
menu_slider_t *buzzer_duty_slider;
menu_slider_t *buzzer_freq_slider;

/* bms subpage */
menu_page_t *menu_sub_bms;
menu_base_t *enter_bms_page;
menu_text_t *bms_current;
menu_text_t *bms_voltage;
menu_text_t *bms_voltage_oc;
menu_text_t *bms_state;

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

menu_text_t *create_menu_text(lv_obj_t *parent, const char *title, const char *content)
{
    menu_text_t *text = lv_mem_alloc(sizeof(menu_text_t));

    text->menu_cont = lv_menu_cont_create(parent);
    lv_obj_set_style_pad_hor(text->menu_cont, MAIN_SCREEN_MENU_PAGE_CONT_PAD_HOR, 0);

    text->title = lv_label_create(text->menu_cont);
    lv_label_set_long_mode(text->title, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_flex_grow(text->title, 1);

    text->content = lv_label_create(text->menu_cont);
    lv_label_set_long_mode(text->content, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_flex_grow(text->content, 0);

    if (title)
        lv_label_set_text(text->title, title);

    if (content)
        lv_label_set_text(text->content, content);

    return text;
}

menu_slider_t *create_menu_slider(lv_obj_t *parent, const char *icon, const char *title, uint32_t min, uint32_t max, uint32_t val, bool hide)
{
    menu_slider_t *obj = lv_mem_alloc(sizeof(menu_slider_t));

    obj->base = create_menu_base(parent, icon, title);
    lv_obj_set_style_pad_hor(obj->base->menu_cont, MAIN_SCREEN_MENU_PAGE_CONT_PAD_HOR, 0);

    obj->val = lv_label_create(obj->base->menu_cont);
    lv_label_set_long_mode(obj->val, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_flex_grow(obj->val, 0);

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

    if (hide)
    {
        lv_label_set_text_fmt(obj->val, "%d", val);
        return obj;
    }

    obj->slider_val = lv_label_create(obj->slider);
    lv_obj_center(obj->slider_val);
    lv_obj_set_style_text_color(obj->slider_val, obj_bg_color, 0);
    lv_label_set_text_fmt(obj->slider_val, "%d", val);

    return obj;
}

menu_switch_t *create_menu_switch(lv_obj_t *parent, const char *icon, const char *title, bool check)
{
    menu_switch_t *obj = lv_mem_alloc(sizeof(menu_switch_t));

    obj->base = create_menu_base(parent, icon, title);
    lv_obj_set_style_pad_hor(obj->base->menu_cont, MAIN_SCREEN_MENU_PAGE_CONT_PAD_HOR, 0);

    obj->sw = lv_switch_create(obj->base->menu_cont);

    lv_obj_add_state(obj->sw, check ? LV_STATE_CHECKED : 0);

    return obj;
}

menu_qrcode_t *create_menu_qrcode(lv_obj_t *parent, const char *icon, const char *title, const char *data)
{
    menu_qrcode_t *obj = lv_mem_alloc(sizeof(menu_qrcode_t));

    obj->base = create_menu_base(parent, icon, title);
    lv_obj_set_style_pad_hor(obj->base->menu_cont, MAIN_SCREEN_MENU_PAGE_CONT_PAD_HOR, 0);

    obj->qr = lv_qrcode_create(obj->base->menu_cont, MAIN_SCREEN_MENU_QR_CODE_SIZE, obj_bg_color, white_color);

    if (data)
        lv_qrcode_update(obj->qr, data, strlen(data));

    lv_obj_center(obj->qr);

    lv_obj_set_style_border_color(obj->qr, white_color, 0);
    lv_obj_set_style_border_width(obj->qr, MAIN_SCREEN_MENU_QR_CODE_BORDER_WIDTH, 0);

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
static void root_back_btn_event_cb(lv_event_t *event)
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
            lv_img_set_src(lv_obj_get_child(lv_menu_get_sidebar_header_back_btn(menu), 0), LV_SYMBOL_LEFT);
        }
        menu_load_page(enter_nw_page);
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
    lv_obj_add_event_cb(main_screen_menu, root_back_btn_event_cb, LV_EVENT_CLICKED, main_screen_menu);

    /* create subpage */
    menu_sub_nw = create_menu_subpage(main_screen_menu, "Network Settings");
    menu_sub_tof = create_menu_subpage(main_screen_menu, "Alarm Threshold");
    menu_sub_buzzer = create_menu_subpage(main_screen_menu, "PWM of Buzzer & LED");
    menu_sub_bms = create_menu_subpage(main_screen_menu, "Battery Information");

    /* create root page */
    menu_root = lv_mem_alloc(sizeof(menu_page_t));
    menu_root->page = lv_menu_page_create(main_screen_menu, "Sidebar");
    lv_obj_set_style_pad_all(menu_root->page, MAIN_SCREEN_MENU_ROOTPAGE_PAD_ALL, 0);

    menu_root->section = lv_menu_section_create(menu_root->page);

    enter_tof_page = create_menu_base(menu_root->section, LV_SYMBOL_USB, "Measure");
    enter_nw_page = create_menu_base(menu_root->section, LV_SYMBOL_HOME, "Service");
    enter_buzzer_page = create_menu_base(menu_root->section, LV_SYMBOL_BELL, "Alarm");
    enter_bms_page = create_menu_base(menu_root->section, LV_SYMBOL_LIST, "Battery");

    /* set the load page event */
    lv_menu_set_load_page_event(main_screen_menu, enter_nw_page->menu_cont, menu_sub_nw->page);
    lv_menu_set_load_page_event(main_screen_menu, enter_tof_page->menu_cont, menu_sub_tof->page);
    lv_menu_set_load_page_event(main_screen_menu, enter_buzzer_page->menu_cont, menu_sub_buzzer->page);
    lv_menu_set_load_page_event(main_screen_menu, enter_bms_page->menu_cont, menu_sub_bms->page);

    /* set siderbar page */
    lv_menu_set_sidebar_page(main_screen_menu, menu_root->page);
    lv_img_set_src(lv_obj_get_child(lv_menu_get_sidebar_header_back_btn(main_screen_menu), 0), LV_SYMBOL_LEFT);
}

static void anim_timer_cb(lv_timer_t *timer)
{
    lv_obj_add_flag(charge_animimg, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(charge_bg, LV_OBJ_FLAG_HIDDEN);
    lv_timer_pause(charge_anim_timer);
}

/*
 * @brief set the charging animation
 * @param parent lv_obj_t *parent object
 */
static void set_lv_charge_anim(lv_obj_t *parent)
{
    charge_bg = lv_obj_create(parent);
    lv_obj_clear_flag(charge_bg, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(charge_bg, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(charge_bg, LV_OBJ_FLAG_HIDDEN);
    lv_obj_remove_style_all(charge_bg);
    lv_obj_add_style(charge_bg, &style_main_screen_bg, 0);
    lv_obj_set_style_opa(charge_bg, LV_OPA_50, 0);

    charge_animimg = lv_animimg_create(parent);
    lv_obj_clear_flag(charge_animimg, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(charge_animimg, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(charge_animimg, LV_OBJ_FLAG_HIDDEN);
    lv_obj_align(charge_animimg, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_opa(charge_animimg, LV_OPA_80, 0);

    lv_animimg_set_src(charge_animimg, (lv_img_dsc_t **)chargeImgs, 12);
    lv_animimg_set_duration(charge_animimg, 1000);
    lv_animimg_set_repeat_count(charge_animimg, 0);

    charge_anim_timer = lv_timer_create(anim_timer_cb, 700, NULL);
    lv_timer_pause(charge_anim_timer);
}

/*
 * @brief set the main screen
 */
void set_lv_content_main()
{
    main_screen = lv_obj_create(NULL);

    main_screen_bg = lv_obj_create(main_screen);
    lv_obj_clear_flag(main_screen_bg, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(main_screen_bg, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_style_all(main_screen_bg);
    lv_obj_add_style(main_screen_bg, &style_main_screen_bg, 0);

    /* create menu on main screen background */
    set_lv_main_screen_menu(main_screen_bg);
    /* create charge animation on top layer */
    set_lv_charge_anim(lv_layer_top());
}

/*
 * @brief show charging animation
 */
void show_lv_charge_anim()
{
    lv_obj_clear_flag(charge_bg, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(charge_animimg, LV_OBJ_FLAG_HIDDEN);
    lv_animimg_start(charge_animimg);
    lv_timer_reset(charge_anim_timer);
    lv_timer_resume(charge_anim_timer);
}

void menu_load_page(menu_base_t *obj)
{
    lv_event_send(obj->menu_cont, LV_EVENT_CLICKED, NULL);
}
