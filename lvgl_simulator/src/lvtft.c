#include "lvtft.h"

lv_obj_t *wifi_ico;
lv_obj_t *wifi_label;

lv_obj_t *battery_label;
lv_obj_t *battery_ico;
lv_obj_t *battery_cg_ico;

lv_obj_t *ui_Screen1;
lv_obj_t *ui_Screen2;

lv_obj_t *ui_Image1;
lv_obj_t *ui_Image2;
lv_obj_t *ui_Image3;

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

    // LV_IMG_DECLARE(img_bulb_gif);
    // lv_obj_t *img;
    // img = lv_gif_create(lv_scr_act());
    // lv_gif_set_src(img, &img_bulb_gif);
    // lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);

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
    static const lv_img_dsc_t *anim_imgs[12] = {
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

    lv_obj_t *animimg0 = lv_animimg_create(lv_scr_act());
    lv_obj_center(animimg0);
    lv_animimg_set_src(animimg0, (lv_img_dsc_t **)anim_imgs, 12);
    lv_animimg_set_duration(animimg0, 1000);
    lv_animimg_set_repeat_count(animimg0, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(animimg0);

    ui_align();
}

void ui_align()
{
    lv_obj_align(wifi_ico, LV_ALIGN_TOP_LEFT, 5, 5);
    lv_obj_align_to(wifi_label, wifi_ico, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_obj_align(battery_label, LV_ALIGN_TOP_RIGHT, -5, 5);
    lv_obj_align_to(battery_ico, battery_label, LV_ALIGN_OUT_LEFT_MID, 0, 0);
    lv_obj_align_to(battery_cg_ico, battery_ico, LV_ALIGN_OUT_LEFT_MID, 0, 0);
}
