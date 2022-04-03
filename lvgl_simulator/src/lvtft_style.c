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

static void set_lv_style_top_status_bar()
{
    /* Create a style for the top status bar */
    lv_style_init(&style_top_status_bar);

    lv_style_set_x(&style_top_status_bar, TOP_STATUS_BAR_X);
    lv_style_set_y(&style_top_status_bar, TOP_STATUS_BAR_Y);
    lv_style_set_width(&style_top_status_bar, TOP_STATUS_BAR_WIDTH);
    lv_style_set_height(&style_top_status_bar, TOP_STATUS_BAR_HEIGHT);

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
}

static void set_lv_style_bottom_status_bar()
{
    /* Create a style for the bottom status bar */
    lv_style_init(&style_bottom_status_bar);

    lv_style_set_x(&style_bottom_status_bar, BOTTOM_STATUS_BAR_X);
    lv_style_set_y(&style_bottom_status_bar, BOTTOM_STATUS_BAR_Y);
    lv_style_set_width(&style_bottom_status_bar, BOTTOM_STATUS_BAR_WIDTH);
    lv_style_set_height(&style_bottom_status_bar, BOTTOM_STATUS_BAR_HEIGHT);

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
}

static void set_lv_style_main_screen_bg()
{
    /* Create a style for the main screen */
    lv_style_init(&style_main_screen_bg);
    lv_style_set_x(&style_main_screen_bg, MAIN_SCREEN_X);
    lv_style_set_y(&style_main_screen_bg, MAIN_SCREEN_Y);

    lv_style_set_width(&style_main_screen_bg, MAIN_SCREEN_WIDTH);
    lv_style_set_height(&style_main_screen_bg, MAIN_SCREEN_HEIGHT);

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

/* Set color and style variable */
void set_lv_style()
{
    set_lv_style_color();

    set_lv_style_top_status_bar();

    set_lv_style_bottom_status_bar();

    set_lv_style_main_screen_bg();
}