#include "ESP32Encoder.h"
#include "TFT_eSPI.h"

#include "lvtft_conf.h"
#include "lvtft_style.h"
#include "lvtft.hpp"

#define ENCODER_BUTTON_PIN 25
#define ENCODER_A_PIN 26
#define ENCODER_B_PIN 27

static int16_t encoder_count;
static int16_t encoder_last_count;
static ESP32Encoder encoder = ESP32Encoder(false, nullptr, nullptr);

static TFT_eSPI tft = TFT_eSPI();

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ScreenWidth * 10];

static lv_disp_drv_t disp_drv;
static lv_disp_t *disp;

static lv_indev_drv_t indev_drv;
static lv_indev_t *indev;

static lv_group_t *indev_group;

#if LV_USE_LOG != 0
static void my_log_cb(const char *buf)
{
    Serial.printf("%s\r\n", buf);
    Serial.flush();
}
#endif

static void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();
    lv_disp_flush_ready(disp);
}

static void set_encoder()
{
    pinMode(ENCODER_BUTTON_PIN, INPUT_PULLUP);
    ESP32Encoder::useInternalWeakPullResistors = UP;
    encoder.attachFullQuad(ENCODER_A_PIN, ENCODER_B_PIN);
    encoder.clearCount();
}

static void encoder_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    encoder_count = encoder.getCount();

    data->enc_diff = encoder_last_count - encoder_count;

    encoder_last_count = encoder_count;

    if (digitalRead(ENCODER_BUTTON_PIN) == HIGH)
        data->state = LV_INDEV_STATE_RELEASED;
    else
        data->state = LV_INDEV_STATE_PRESSED;
}

static void set_lv_drv()
{
    lv_init();

#if LV_USE_LOG != 0
    lv_log_register_print_cb(my_log_cb);
#endif

    lv_disp_draw_buf_init(&draw_buf, buf, NULL, ScreenWidth * 10);

    tft.begin();
    tft.setRotation(1);

    /*Initialize the display driver*/
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = ScreenWidth;
    disp_drv.ver_res = ScreenHeight;
    disp_drv.rotated = LV_DISP_ROT_NONE;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;

    disp = lv_disp_drv_register(&disp_drv);

    /*Initialize the input device driver*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = encoder_read;

    indev = lv_indev_drv_register(&indev_drv);

    /*Initialize the input device group*/
    indev_group = lv_group_create();
    lv_indev_set_group(indev, indev_group);
    lv_group_set_default(indev_group);
}

static void set_lv_indev_group()
{
    lv_group_add_obj(indev_group, enter_tof_page->menu_cont);
    lv_group_add_obj(indev_group, enter_nw_page->menu_cont);
    lv_group_add_obj(indev_group, enter_buzzer_page->menu_cont);
    lv_group_add_obj(indev_group, enter_bms_page->menu_cont);
}

void set_lvgl()
{
    set_encoder();

    set_lv_drv();

    set_lv_style();

    set_lv_top_status_bar();

    set_lv_bottom_status_bar();

    set_lv_main_screen();

    set_lv_indev_group();

    lv_scr_load(main_screen);
}