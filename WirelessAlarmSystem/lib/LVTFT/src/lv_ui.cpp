#include "lv_ui.h"

lv_group_t *encoderGroup;

/* mainScreen */
lv_obj_t *mainScreen;

lv_obj_t *wifi_ico;
lv_obj_t *wifi_label;

lv_obj_t *battery_label;
lv_obj_t *battery_ico;
lv_obj_t *battery_cg_ico;

/* chargeScreen */
lv_obj_t *chargeScreen;
lv_obj_t *chargeAnimimg;

/* test */
lv_obj_t *tof_label;
lv_obj_t *bt_label;

void set_lv_mainScreen()
{
    /* mainScreen */
    mainScreen = lv_obj_create(NULL);
    // lv_obj_clear_flag(mainScreen, LV_OBJ_FLAG_SCROLLABLE);

    wifi_ico = lv_label_create(mainScreen);
    lv_obj_set_style_text_font(wifi_ico, &lv_font_montserrat_12, 0);
    lv_label_set_text(wifi_ico, LV_SYMBOL_WIFI);

    wifi_label = lv_label_create(mainScreen);
    lv_obj_set_style_text_font(wifi_label, &lv_font_montserrat_12, 0);

    battery_label = lv_label_create(mainScreen);
    lv_obj_set_style_text_font(battery_label, &lv_font_montserrat_12, 0);

    battery_ico = lv_label_create(mainScreen);
    lv_obj_set_style_text_font(battery_ico, &lv_font_montserrat_12, 0);

    battery_cg_ico = lv_label_create(mainScreen);
    lv_obj_set_style_text_font(battery_cg_ico, &lv_font_montserrat_12, 0);
    lv_label_set_recolor(battery_cg_ico, true);
    lv_obj_add_flag(battery_cg_ico, LV_OBJ_FLAG_HIDDEN);

    tof_label = lv_label_create(mainScreen);
    lv_obj_set_style_text_font(tof_label, &lv_font_simsun_16_cjk, 0);

    bt_label = lv_label_create(mainScreen);

    mainScreenAlign();
}

void mainScreenAlign()
{
    lv_obj_align(wifi_ico, LV_ALIGN_TOP_LEFT, 5, 5);
    lv_obj_align(battery_label, LV_ALIGN_TOP_RIGHT, -5, 5);
    lv_obj_align(tof_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(bt_label, LV_ALIGN_BOTTOM_MID, 0, -10);

    lv_obj_align_to(wifi_label, wifi_ico, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_obj_align_to(battery_ico, battery_label, LV_ALIGN_OUT_LEFT_MID, 0, 0);
    lv_obj_align_to(battery_cg_ico, battery_ico, LV_ALIGN_OUT_LEFT_MID, 0, 0);
}

static void chargeScreen_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        LV_LOG_USER("Clicked");
        lv_group_remove_obj(chargeScreen);
        lv_scr_load_anim(mainScreen, LV_SCR_LOAD_ANIM_FADE_ON, 100, 50, false);
    }
}

void set_lv_chargeScreen()
{
    /* chargeScreen */
    chargeScreen = lv_obj_create(NULL);
    // lv_obj_clear_flag(chargeScreen, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_event_cb(chargeScreen, chargeScreen_handler, LV_EVENT_CLICKED, NULL);

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

    chargeAnimimg = lv_animimg_create(chargeScreen);
    lv_obj_center(chargeAnimimg);
    lv_animimg_set_src(chargeAnimimg, (lv_img_dsc_t **)chargeImgs, 12);
    lv_animimg_set_duration(chargeAnimimg, 1000);
    lv_animimg_set_repeat_count(chargeAnimimg, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(chargeAnimimg);
}

void set_encoderGroup()
{
    encoderGroup = lv_group_create();
    lv_indev_set_group(indev, encoderGroup);
    lv_group_set_default(encoderGroup);

    // lv_group_add_obj(encoderGroup, chargeScreen);
}
