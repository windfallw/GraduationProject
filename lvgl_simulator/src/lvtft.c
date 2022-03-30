#include "lvtft.h"

/********************
 *      DEFINES     *
 ********************/

/* Screen Size */
#define ScreenWidth SDL_HOR_RES
#define ScreenHeight SDL_VER_RES

/* 4 main color from https://colordrop.io */
#define LIGHT_COLOR 0x08ffc8
#define WHITE_COLOR 0xfff7f7
#define DARK_COLOR 0xdadada
#define OBJ_BG_COLOR 0x204969

/* Status Bar */
#define TOP_STATUS_BAR_SIZE 5
#define TOP_STATUS_BAR_PAD 5
#define TOP_STATUS_BAR_FONT lv_font_montserrat_12
#define TOP_STATUS_BAR_FONT_PAD 1

#define BOTTOM_STATUS_BAR_SIZE 5
#define BOTTOM_STATUS_BAR_PAD 5
#define BOTTOM_STATUS_BAR_FONT lv_font_montserrat_12
#define BOTTOM_STATUS_BAR_FONT_PAD 1

/* Main Screen */

/********************
 *     VARIABLE     *
 ********************/

/* Color */
static lv_color_t light_color;
static lv_color_t white_color;
static lv_color_t dark_color;
static lv_color_t obj_bg_color;

/* Style */
static lv_style_t style_top_status_bar;
static lv_style_t style_top_status_bar_font;

static lv_style_t style_bottom_status_bar;
static lv_style_t style_bottom_status_bar_font;

static lv_style_t style_main_screen_bg;
static lv_style_t style_main_screen_btn;
static lv_style_t style_main_screen_font;

/* Set color and style variable */
static void set_lv_style()
{
    /* Create color */
    light_color = lv_color_hex(LIGHT_COLOR);
    white_color = lv_color_hex(WHITE_COLOR);
    dark_color = lv_color_hex(DARK_COLOR);
    obj_bg_color = lv_color_hex(OBJ_BG_COLOR);

    /***************
     *     top     *
     ***************/

    /* Create a style for the top status bar */
    lv_style_init(&style_top_status_bar);

    lv_style_set_x(&style_top_status_bar, 0);
    lv_style_set_y(&style_top_status_bar, 0);
    lv_style_set_width(&style_top_status_bar, ScreenWidth);
    lv_style_set_height(&style_top_status_bar, TOP_STATUS_BAR_FONT.line_height + TOP_STATUS_BAR_SIZE);

    lv_style_set_radius(&style_top_status_bar, 0);
    lv_style_set_border_width(&style_top_status_bar, 1);
    lv_style_set_pad_all(&style_top_status_bar, TOP_STATUS_BAR_PAD);

    lv_style_set_bg_color(&style_top_status_bar, obj_bg_color);
    lv_style_set_bg_opa(&style_top_status_bar, LV_OPA_90);
    lv_style_set_border_color(&style_top_status_bar, dark_color);

    lv_style_set_shadow_width(&style_top_status_bar, 3);
    lv_style_set_shadow_ofs_x(&style_top_status_bar, 1);
    lv_style_set_shadow_ofs_y(&style_top_status_bar, 1);
    lv_style_set_shadow_opa(&style_top_status_bar, LV_OPA_10);

    /* Create a style for the top status bar's font */
    lv_style_init(&style_top_status_bar_font);
    lv_style_set_text_color(&style_top_status_bar_font, white_color);
    lv_style_set_text_font(&style_top_status_bar_font, &TOP_STATUS_BAR_FONT);
    lv_style_set_pad_all(&style_top_status_bar_font, TOP_STATUS_BAR_FONT_PAD);

    /***************
     *    bottom   *
     ***************/

    /* Create a style for the bottom status bar */
    lv_style_init(&style_bottom_status_bar);

    lv_style_set_x(&style_bottom_status_bar, 0);
    lv_style_set_y(&style_bottom_status_bar, ScreenHeight - (BOTTOM_STATUS_BAR_FONT.line_height + BOTTOM_STATUS_BAR_SIZE));
    lv_style_set_width(&style_bottom_status_bar, ScreenWidth);
    lv_style_set_height(&style_bottom_status_bar, BOTTOM_STATUS_BAR_FONT.line_height + BOTTOM_STATUS_BAR_SIZE);

    lv_style_set_radius(&style_bottom_status_bar, 0);
    lv_style_set_border_width(&style_bottom_status_bar, 1);
    lv_style_set_pad_all(&style_bottom_status_bar, BOTTOM_STATUS_BAR_PAD);

    lv_style_set_bg_color(&style_bottom_status_bar, obj_bg_color);
    lv_style_set_bg_opa(&style_bottom_status_bar, LV_OPA_90);
    lv_style_set_border_color(&style_bottom_status_bar, dark_color);

    lv_style_set_shadow_width(&style_bottom_status_bar, 3);
    lv_style_set_shadow_ofs_x(&style_bottom_status_bar, -1);
    lv_style_set_shadow_ofs_y(&style_bottom_status_bar, -1);
    lv_style_set_shadow_opa(&style_bottom_status_bar, LV_OPA_10);

    /* Create a style for the bottom status bar's font */
    lv_style_init(&style_bottom_status_bar_font);
    lv_style_set_text_color(&style_bottom_status_bar_font, white_color);
    lv_style_set_text_font(&style_bottom_status_bar_font, &BOTTOM_STATUS_BAR_FONT);
    lv_style_set_pad_all(&style_bottom_status_bar_font, BOTTOM_STATUS_BAR_FONT_PAD);

    /********************
     *    main screen   *
     ********************/

    /* Create a style for the main screen */
    lv_style_init(&style_main_screen_bg);
    lv_style_set_x(&style_main_screen_bg, 0);
    lv_style_set_y(&style_main_screen_bg, TOP_STATUS_BAR_FONT.line_height + TOP_STATUS_BAR_SIZE);

    lv_style_set_width(&style_main_screen_bg, ScreenWidth);
    lv_style_set_height(&style_main_screen_bg, ScreenHeight - (TOP_STATUS_BAR_FONT.line_height + TOP_STATUS_BAR_SIZE + BOTTOM_STATUS_BAR_FONT.line_height + BOTTOM_STATUS_BAR_SIZE));

    lv_style_set_radius(&style_main_screen_bg, 0);
    lv_style_set_border_width(&style_main_screen_bg, 3);

    lv_style_set_bg_color(&style_main_screen_bg, white_color);
    lv_style_set_bg_opa(&style_main_screen_bg, LV_OPA_100);
    lv_style_set_border_color(&style_main_screen_bg, dark_color);

    lv_style_set_shadow_width(&style_main_screen_bg, 5);
    lv_style_set_shadow_ofs_x(&style_main_screen_bg, 0);
    lv_style_set_shadow_ofs_y(&style_main_screen_bg, 0);
    lv_style_set_shadow_opa(&style_main_screen_bg, LV_OPA_10);
}

/********************
 *     LVGL OBJ     *
 ********************/

/* Top Screen Status Bar */
lv_obj_t *top_status_bar;

lv_obj_t *wifi_ico;
lv_obj_t *wifi_txt;

lv_obj_t *battery_level_txt;
lv_obj_t *battery_level_ico;
lv_obj_t *battery_lightning_ico;

/* Bottom Status Bar */
lv_obj_t *bottom_status_bar;

/* Main Screen */
lv_obj_t *main_screen;
lv_obj_t *main_screen_bg;

/* Charge Screen */
lv_obj_t *charge_screen;
lv_obj_t *charge_animimg;
lv_obj_t *charge_txt[3];

/********************
 *     FUNCTION     *
 ********************/

/*
 * @brief set the status bar on lv_layer_top()
 */
static void set_lv_status_bar()
{
    /********************
     *      TOP BAR      *
     ********************/

    top_status_bar = lv_obj_create(lv_layer_top());
    lv_obj_clear_flag(top_status_bar, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(top_status_bar, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_style_all(top_status_bar);
    lv_obj_add_style(top_status_bar, &style_top_status_bar, 0);

    /* wifi icon */
    wifi_ico = lv_label_create(top_status_bar);
    lv_obj_add_style(wifi_ico, &style_top_status_bar_font, 0);
    lv_label_set_text(wifi_ico, LV_SYMBOL_WIFI);

    /* wifi text */
    wifi_txt = lv_label_create(top_status_bar);
    lv_obj_add_style(wifi_txt, &style_top_status_bar_font, 0);

    /* battery level txt */
    battery_level_txt = lv_label_create(top_status_bar);
    lv_obj_add_style(battery_level_txt, &style_top_status_bar_font, 0);

    /* battery level icon */
    battery_level_ico = lv_label_create(top_status_bar);
    lv_obj_add_style(battery_level_ico, &style_top_status_bar_font, 0);

    /* battery lightning icon */
    battery_lightning_ico = lv_label_create(top_status_bar);
    lv_obj_add_flag(battery_lightning_ico, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_style(battery_lightning_ico, &style_top_status_bar_font, 0);
    lv_obj_set_style_text_color(battery_lightning_ico, light_color, 0);
    lv_label_set_text(battery_lightning_ico, LV_SYMBOL_CHARGE);

    /********************
     *    BOTTOM BAR    *
     ********************/

    bottom_status_bar = lv_obj_create(lv_layer_top());
    lv_obj_clear_flag(bottom_status_bar, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(bottom_status_bar, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_style_all(bottom_status_bar);
    lv_obj_add_style(bottom_status_bar, &style_bottom_status_bar, 0);

    /******************
     *     ALIGN      *
     ******************/
    lv_obj_align(wifi_ico, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_align(battery_level_txt, LV_ALIGN_RIGHT_MID, 0, 0);

    align_lv_top_status_bar();
}

/*
 * @brief align the top status bar
 */
void align_lv_top_status_bar()
{
    lv_obj_align_to(wifi_txt, wifi_ico, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_obj_align_to(battery_level_ico, battery_level_txt, LV_ALIGN_OUT_LEFT_MID, 0, 0);
    lv_obj_align_to(battery_lightning_ico, battery_level_ico, LV_ALIGN_OUT_LEFT_MID, 0, 0);
}

static void set_lv_main_screen()
{
    /* mainScreen */
    main_screen = lv_obj_create(NULL);

    main_screen_bg = lv_obj_create(main_screen);
    lv_obj_clear_flag(main_screen_bg, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(main_screen_bg, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_style_all(main_screen_bg);
    lv_obj_add_style(main_screen_bg, &style_main_screen_bg, 0);
}

static void charge_screen_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        LV_LOG_USER("Clicked");
        lv_scr_load_anim(main_screen, LV_SCR_LOAD_ANIM_FADE_ON, 100, 50, false);
    }
}

static void set_lv_charge_screen()
{
    lv_scr_act();
    /* Charge Screen */
    charge_screen = lv_obj_create(NULL);
    lv_obj_clear_flag(charge_screen, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_event_cb(charge_screen, charge_screen_handler, LV_EVENT_CLICKED, NULL);

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

    charge_animimg = lv_animimg_create(charge_screen);
    lv_obj_align(charge_animimg, LV_ALIGN_CENTER, 0, -30);
    lv_animimg_set_src(charge_animimg, (lv_img_dsc_t **)chargeImgs, 12);
    lv_animimg_set_duration(charge_animimg, 1000);
    lv_animimg_set_repeat_count(charge_animimg, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(charge_animimg);

    charge_txt[0] = lv_label_create(charge_screen);
    lv_obj_align(charge_txt[0], LV_ALIGN_CENTER, 0, 30);
    lv_obj_set_style_text_font(charge_txt[0], &lv_font_montserrat_12, 0);
    lv_label_set_recolor(charge_txt[0], true);

    lv_label_set_text(charge_txt[0], "#0000ff VoltageOC: 4300mV");

    charge_txt[1] = lv_label_create(charge_screen);
    lv_obj_align(charge_txt[1], LV_ALIGN_CENTER, 0, 50);
    lv_obj_set_style_text_font(charge_txt[1], &lv_font_montserrat_12, 0);
    lv_label_set_recolor(charge_txt[1], true);

    lv_label_set_text(charge_txt[1], "#ff0000 Voltage: 4300mV");
}

/*
 * @brief set lvgl and load main screen
 */
void set_lvgl()
{
    set_lv_style();

    set_lv_status_bar();
    set_lv_main_screen();
    set_lv_charge_screen();
}