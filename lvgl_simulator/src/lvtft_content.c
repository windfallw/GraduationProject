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

enum
{
    LV_MENU_ITEM_BUILDER_VARIANT_1,
    LV_MENU_ITEM_BUILDER_VARIANT_2
};

static lv_obj_t *create_text(lv_obj_t *parent, const char *icon, const char *txt, uint8_t builder_variant)
{
    lv_obj_t *obj = lv_menu_cont_create(parent);

    lv_obj_t *img = NULL;
    lv_obj_t *label = NULL;

    if (icon)
    {
        img = lv_img_create(obj);
        lv_img_set_src(img, icon);
    }

    if (txt)
    {
        label = lv_label_create(obj);

        lv_label_set_text(label, txt);
        lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_flex_grow(label, 1);
    }

    if (builder_variant == LV_MENU_ITEM_BUILDER_VARIANT_2 && icon && txt)
    {
        lv_obj_add_flag(img, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
        lv_obj_swap(img, label);
    }

    return obj;
}

static lv_obj_t *create_slider(lv_obj_t *parent, const char *icon, const char *txt, int32_t min, int32_t max, int32_t val)
{
    lv_obj_t *obj = create_text(parent, icon, txt, LV_MENU_ITEM_BUILDER_VARIANT_2);

    lv_obj_t *slider = lv_slider_create(obj);

    // set the slider to react on dragging the knob only
    lv_obj_add_flag(slider, LV_OBJ_FLAG_ADV_HITTEST);

    lv_obj_set_style_radius(slider, LV_RADIUS_CIRCLE, LV_PART_KNOB);
    lv_obj_set_style_pad_all(slider, 2, LV_PART_KNOB);

    lv_obj_set_flex_grow(slider, 1);

    lv_slider_set_mode(slider, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_range(slider, min, max);
    lv_slider_set_value(slider, val, LV_ANIM_OFF);

    if (icon == NULL)
    {
        lv_obj_add_flag(slider, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    }

    lv_obj_set_style_pad_hor(obj, 10, 0);

    return obj;
}

static lv_obj_t *create_switch(lv_obj_t *parent, const char *icon, const char *txt, bool chk)
{
    lv_obj_t *obj = create_text(parent, icon, txt, LV_MENU_ITEM_BUILDER_VARIANT_1);

    lv_obj_t *sw = lv_switch_create(obj);

    lv_obj_add_state(sw, chk ? LV_STATE_CHECKED : 0);

    lv_obj_set_style_pad_hor(obj, 10, 0);

    return obj;
}

static void root_back_btn_handler(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    lv_obj_t *menu = lv_event_get_user_data(e);

    if (lv_menu_back_btn_is_root(menu, obj))
    {
        if (lv_menu_get_cur_sidebar_page(menu) == menu_root_page)
        {

            lv_menu_set_sidebar_page(menu, NULL);
            lv_menu_clear_history(menu);
            lv_menu_set_page(menu, menu_root_page);
        }
        else
        {
            lv_menu_set_page(menu, NULL);
            lv_menu_set_sidebar_page(menu, menu_root_page);
            // set sidebar header icon after sidebar page is set
            // lv_img_set_src(lv_obj_get_child(lv_menu_get_sidebar_header_back_btn(menu), 0), LV_SYMBOL_REFRESH);
            lv_event_send(tof_page_enter, LV_EVENT_CLICKED, NULL);
        }
    }
}

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

    /* create sub page tof */
    tof_page = lv_menu_page_create(main_screen_menu, "Alarm Threshold");

    lv_menu_separator_create(tof_page);
    lv_obj_set_style_pad_hor(tof_page, 6, 0);

    tof_page_section = lv_menu_section_create(tof_page);

    tof_limit_slider1 = create_slider(tof_page_section, NULL, "tof lidar 1", 0, 20000, 1000);
    tof_limit_slider2 = create_slider(tof_page_section, NULL, "tof lidar 2", 0, 20000, 2333);

    /* create sub page buzzer */
    buzzer_page = lv_menu_page_create(main_screen_menu, "PWM of Buzzer & LED");

    lv_menu_separator_create(buzzer_page);
    lv_obj_set_style_pad_hor(buzzer_page, 6, 0);

    buzzer_page_section = lv_menu_section_create(buzzer_page);

    buzzer_mute_switch = create_switch(buzzer_page_section, LV_SYMBOL_AUDIO, "mute", false);

    buzzer_duty_slider = create_slider(buzzer_page_section, NULL, "duty cycle", 0, 150, 120);

    buzzer_freq_slider = create_slider(buzzer_page_section, NULL, "frequency", 0, 150, 50);

    /* create root page */
    menu_root_page = lv_menu_page_create(main_screen_menu, "Sidebar");

    lv_obj_set_style_pad_all(menu_root_page, 0, 0);

    root_page_section = lv_menu_section_create(menu_root_page);

    tof_page_enter = create_text(root_page_section, LV_SYMBOL_SETTINGS, "Measure", LV_MENU_ITEM_BUILDER_VARIANT_1);
    buzzer_page_enter = create_text(root_page_section, LV_SYMBOL_SETTINGS, "Alarm", LV_MENU_ITEM_BUILDER_VARIANT_1);

    lv_menu_set_load_page_event(main_screen_menu, tof_page_enter, tof_page);
    lv_menu_set_load_page_event(main_screen_menu, buzzer_page_enter, buzzer_page);

    /* set siderbar page */
    lv_menu_set_sidebar_page(main_screen_menu, menu_root_page);
    // set sidebar header icon after sidebar page is set
    // lv_img_set_src(lv_obj_get_child(lv_menu_get_sidebar_header_back_btn(main_screen_menu), 0), LV_SYMBOL_REFRESH);
    lv_event_send(tof_page_enter, LV_EVENT_CLICKED, NULL);
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
