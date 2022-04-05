#include "lvtft_conf.h"
#include "lvtft_style.h"

static void set_lv_style_color()
{
    /* Create color */
    light_color = lv_color_hex(LIGHT_COLOR);
    white_color = lv_color_hex(WHITE_COLOR);
    dark_color = lv_color_hex(DARK_COLOR);
    obj_bg_color = lv_color_hex(OBJ_BG_COLOR);
}

static void set_lv_style_top_status_bar(lv_style_t *style, lv_style_t *font)
{
    /* Create a style for the top status bar */
    lv_style_init(style);

    lv_style_set_x(style, TOP_STATUS_BAR_X);
    lv_style_set_y(style, TOP_STATUS_BAR_Y);
    lv_style_set_width(style, TOP_STATUS_BAR_WIDTH);
    lv_style_set_height(style, TOP_STATUS_BAR_HEIGHT);

    lv_style_set_radius(style, 0);
    lv_style_set_border_width(style, 1);
    lv_style_set_pad_all(style, TOP_STATUS_BAR_PAD);

    lv_style_set_bg_color(style, obj_bg_color);
    lv_style_set_bg_opa(style, LV_OPA_90);
    lv_style_set_border_color(style, dark_color);

    lv_style_set_shadow_width(style, 3);
    lv_style_set_shadow_ofs_x(style, 1);
    lv_style_set_shadow_ofs_y(style, 1);
    lv_style_set_shadow_opa(style, LV_OPA_10);

    /* Create a style for the top status bar's font */
    lv_style_init(font);
    lv_style_set_text_color(font, white_color);
    lv_style_set_text_font(font, &TOP_STATUS_BAR_FONT);
    lv_style_set_pad_all(font, TOP_STATUS_BAR_FONT_PAD);
}

static void set_lv_style_bottom_status_bar(lv_style_t *style, lv_style_t *font)
{
    /* Create a style for the bottom status bar */
    lv_style_init(style);

    lv_style_set_x(style, BOTTOM_STATUS_BAR_X);
    lv_style_set_y(style, BOTTOM_STATUS_BAR_Y);
    lv_style_set_width(style, BOTTOM_STATUS_BAR_WIDTH);
    lv_style_set_height(style, BOTTOM_STATUS_BAR_HEIGHT);

    lv_style_set_radius(style, 0);
    lv_style_set_border_width(style, 1);
    lv_style_set_pad_all(style, BOTTOM_STATUS_BAR_PAD);

    lv_style_set_bg_color(style, obj_bg_color);
    lv_style_set_bg_opa(style, LV_OPA_90);
    lv_style_set_border_color(style, dark_color);

    lv_style_set_shadow_width(style, 3);
    lv_style_set_shadow_ofs_x(style, -1);
    lv_style_set_shadow_ofs_y(style, -1);
    lv_style_set_shadow_opa(style, LV_OPA_10);

    /* Create a style for the bottom status bar's font */
    lv_style_init(font);
    lv_style_set_text_color(font, white_color);
    lv_style_set_text_font(font, &BOTTOM_STATUS_BAR_FONT);
    lv_style_set_pad_all(font, BOTTOM_STATUS_BAR_FONT_PAD);
}

static void set_lv_style_main_screen_bg(lv_style_t *style)
{
    /* Create a style for the main screen */
    lv_style_init(style);
    lv_style_set_x(style, MAIN_SCREEN_X);
    lv_style_set_y(style, MAIN_SCREEN_Y);

    lv_style_set_width(style, MAIN_SCREEN_WIDTH);
    lv_style_set_height(style, MAIN_SCREEN_HEIGHT);

    lv_style_set_radius(style, 0);
    lv_style_set_border_width(style, 1);

    lv_style_set_bg_color(style, white_color);
    lv_style_set_bg_opa(style, LV_OPA_100);
    lv_style_set_border_color(style, dark_color);

    lv_style_set_shadow_width(style, 5);
    lv_style_set_shadow_ofs_x(style, 0);
    lv_style_set_shadow_ofs_y(style, 0);
    lv_style_set_shadow_opa(style, LV_OPA_10);
}

static void set_lv_style_main_screen_menu(lv_style_t *style)
{
    lv_style_init(style);

    lv_style_set_align(style, LV_ALIGN_CENTER);

    lv_style_set_width(style, lv_pct(100));
    lv_style_set_height(style, lv_pct(100));

    lv_style_set_radius(style, 0);
    lv_style_set_border_width(style, 0);

    lv_style_set_bg_color(style, lv_color_white());
    lv_style_set_bg_opa(style, LV_OPA_20);

    lv_style_set_text_font(style, &MAIN_SCREEN_MENU_FONT);
}

/* Set color and style variable */
void set_lv_style()
{
    set_lv_style_color();

    set_lv_style_top_status_bar(&style_top_status_bar, &style_top_status_bar_font);

    set_lv_style_bottom_status_bar(&style_bottom_status_bar, &style_bottom_status_bar_font);

    set_lv_style_main_screen_bg(&style_main_screen_bg);

    set_lv_style_main_screen_menu(&style_main_screen_menu);
}