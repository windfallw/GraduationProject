#include "AiEsp32RotaryEncoder.h"
#include "TFT_eSPI.h"

#include "lvtft_conf.h"
#include "lvtft_style.h"
#include "lvtft.hpp"

#define ROTARY_ENCODER_BUTTON_PIN 25
#define ROTARY_ENCODER_A_PIN 27
#define ROTARY_ENCODER_B_PIN 26
#define ROTARY_ENCODER_VCC_PIN -1
#define ROTARY_ENCODER_STEPS 4

static AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
static TFT_eSPI tft = TFT_eSPI();

static lv_disp_t *disp;
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

static void IRAM_ATTR readEncoderISR()
{
    rotaryEncoder.readEncoder_ISR();
}

static void set_rotary_encoder()
{
    rotaryEncoder.begin();
    rotaryEncoder.setup(readEncoderISR);
    rotaryEncoder.setBoundaries(0, 10000, true);
    rotaryEncoder.setAcceleration(150);
    // rotaryEncoder.setEncoderValue(0);
}

static void encoder_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    data->enc_diff = rotaryEncoder.encoderChanged();
    Serial.println(data->enc_diff);
    if (rotaryEncoder.isEncoderButtonDown())
        data->state = LV_INDEV_STATE_PRESSED;
    else
        data->state = LV_INDEV_STATE_RELEASED;
}

static void set_lv_drv()
{
    /*Initialize lvgl*/
    lv_init();

#if LV_USE_LOG != 0
    lv_log_register_print_cb(my_log_cb);
#endif

    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t buf[ScreenWidth * 10];
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, ScreenWidth * 10);

    /*Initialize the display*/
    tft.begin();
    tft.setRotation(1);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = ScreenWidth;
    disp_drv.ver_res = ScreenHeight;
    disp_drv.rotated = LV_DISP_ROT_NONE;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;

    disp = lv_disp_drv_register(&disp_drv);

    /*Initialize the input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = encoder_read;

    indev = lv_indev_drv_register(&indev_drv);
}

static void set_lv_indev_group()
{
    indev_group = lv_group_create();
    lv_indev_set_group(indev, indev_group);
    lv_group_set_default(indev_group);
    // lv_group_add_obj(indev_group, enter_tof_page->menu_cont);
    // lv_group_add_obj(indev_group, enter_buzzer_page->menu_cont);
}

void set_lvgl()
{
    set_rotary_encoder();

    set_lv_drv();

    set_lv_indev_group();

    set_lv_style();

    set_lv_top_status_bar();

    set_lv_bottom_status_bar();

    set_lv_main_screen();

    set_lv_charge_screen();

    lv_scr_load(main_screen);
}