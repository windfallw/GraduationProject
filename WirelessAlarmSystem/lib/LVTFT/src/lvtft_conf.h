#ifndef LVTFT_CONF_H
#define LVTFT_CONF_H

/********************
 *      DEFINES     *
 ********************/

/* Screen Size */
/* TFT_ESPI reverse height and width in lvgl */
#define ScreenWidth TFT_HEIGHT
#define ScreenHeight TFT_WIDTH

/* 4 main color from https://colordrop.io */
#define LIGHT_COLOR 0x08ffc8
#define WHITE_COLOR 0xfff7f7
#define DARK_COLOR 0xdadada
#define OBJ_BG_COLOR 0x204969

/* Top Status Bar */
#define TOP_STATUS_BAR_PAD_ALL 4
#define TOP_STATUS_BAR_EXTRA_HEIGHT 6

#define TOP_STATUS_BAR_FONT lv_font_montserrat_12
#define TOP_STATUS_BAR_FONT_PAD_HOR 2

#define TOP_STATUS_BAR_WIDTH ScreenWidth
#define TOP_STATUS_BAR_HEIGHT (TOP_STATUS_BAR_FONT.line_height + TOP_STATUS_BAR_EXTRA_HEIGHT)

#define TOP_STATUS_BAR_X 0
#define TOP_STATUS_BAR_Y 0

/* Bottom Status Bar */
#define BOTTOM_STATUS_BAR_PAD_ALL 4
#define BOTTOM_STATUS_BAR_EXTRA_HEIGHT 6

#define BOTTOM_STATUS_BAR_FONT lv_font_montserrat_12
#define BOTTOM_STATUS_BAR_FONT_PAD_HOR 2

#define BOTTOM_STATUS_BAR_WIDTH ScreenWidth
#define BOTTOM_STATUS_BAR_HEIGHT (BOTTOM_STATUS_BAR_FONT.line_height + BOTTOM_STATUS_BAR_EXTRA_HEIGHT)

#define BOTTOM_STATUS_BAR_X 0
#define BOTTOM_STATUS_BAR_Y (ScreenHeight - BOTTOM_STATUS_BAR_HEIGHT)

/* Main Screen */
#define MAIN_SCREEN_X 0
#define MAIN_SCREEN_Y TOP_STATUS_BAR_HEIGHT

#define MAIN_SCREEN_WIDTH ScreenWidth
#define MAIN_SCREEN_HEIGHT (ScreenHeight - (TOP_STATUS_BAR_HEIGHT + BOTTOM_STATUS_BAR_HEIGHT))

#define MAIN_SCREEN_MENU_FONT lv_font_montserrat_12
#define MAIN_SCREEN_MENU_ROOTPAGE_PAD_ALL 0
#define MAIN_SCREEN_MENU_SUBPAGE_PAD_HOR 6
#define MAIN_SCREEN_MENU_PAGE_CONT_PAD_HOR 10

/* Others */
#define MAIN_SCREEN_MENU_QR_CODE_SIZE 80
#define MAIN_SCREEN_MENU_QR_CODE_BORDER_WIDTH 3

#endif