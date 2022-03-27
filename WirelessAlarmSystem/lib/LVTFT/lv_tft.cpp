#include "lv_tft.h"
// #include "charging.c"

#define ROTARY_ENCODER_BUTTON_PIN 25
#define ROTARY_ENCODER_A_PIN 26
#define ROTARY_ENCODER_B_PIN 27
#define ROTARY_ENCODER_VCC_PIN -1
#define ROTARY_ENCODER_STEPS 4

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);

TFT_eSPI tft = TFT_eSPI();

// TFT_ESPI reverse height and width in lvgl
static constexpr const uint32_t screenWidth = TFT_HEIGHT;
static constexpr const uint32_t screenHeight = TFT_WIDTH;

lv_disp_draw_buf_t draw_buf;
lv_color_t buf[screenWidth * 10];

lv_disp_t *disp;
lv_indev_t *indev;

lv_obj_t *wifi_ico;
lv_obj_t *wifi_label;

lv_obj_t *battery_label;
lv_obj_t *battery_ico;
lv_obj_t *battery_cg_ico;

lv_obj_t *tof_label;
lv_obj_t *bt_label;

#if LV_USE_LOG != 0
void my_log_cb(const char *buf)
{
    Serial.printf("%s\r\n", buf);
    Serial.flush();
}
#endif

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();
    lv_disp_flush_ready(disp);
}

void IRAM_ATTR readEncoderISR()
{
    rotaryEncoder.readEncoder_ISR();
}

void set_rotary_encoder()
{
    rotaryEncoder.begin();
    rotaryEncoder.setup(readEncoderISR);
    bool circleValues = false;
    rotaryEncoder.setBoundaries(-1000, 1000, circleValues);
    rotaryEncoder.setAcceleration(50);
    // rotaryEncoder.setEncoderValue(0);
}

void encoder_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    data->enc_diff = rotaryEncoder.encoderChanged();
    // Serial.println(data->enc_diff);
    if (rotaryEncoder.isEncoderButtonDown())
        data->state = LV_INDEV_STATE_PRESSED;
    else
        data->state = LV_INDEV_STATE_RELEASED;
}

void set_disp_drv()
{
    /*Initialize lvgl*/
    lv_init();

#if LV_USE_LOG != 0
    lv_log_register_print_cb(my_log_cb);
#endif

    lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);

    /*Initialize the display*/
    tft.begin();
    tft.setRotation(1);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
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

void set_ui()
{
    /* Ui */
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

    /* rotary encoder */
    lv_group_t *g = lv_group_create();
    lv_indev_set_group(indev, g);
    // lv_group_add_obj(g, btn1);

    tof_label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(tof_label, &lv_font_simsun_16_cjk, 0);

    bt_label = lv_label_create(lv_scr_act());

    lv_obj_align_all();
}

void lv_obj_align_all()
{
    lv_obj_align(wifi_ico, LV_ALIGN_TOP_LEFT, 5, 5);
    lv_obj_align_to(wifi_label, wifi_ico, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_obj_align(battery_label, LV_ALIGN_TOP_RIGHT, -5, 5);
    lv_obj_align_to(battery_ico, battery_label, LV_ALIGN_OUT_LEFT_MID, 0, 0);
    lv_obj_align_to(battery_cg_ico, battery_ico, LV_ALIGN_OUT_LEFT_MID, 0, 0);

    lv_obj_align(tof_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(bt_label, LV_ALIGN_BOTTOM_MID, 0, -10);
}