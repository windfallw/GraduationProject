#include "lv_tft.h"

#define ROTARY_ENCODER_BUTTON_PIN 25
#define ROTARY_ENCODER_A_PIN 26
#define ROTARY_ENCODER_B_PIN 27
#define ROTARY_ENCODER_VCC_PIN -1
#define ROTARY_ENCODER_STEPS 4

// TFT_ESPI reverse height and width in lvgl
#define screenWidth TFT_HEIGHT
#define screenHeight TFT_WIDTH

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);

TFT_eSPI tft = TFT_eSPI();

lv_disp_t *disp;
lv_indev_t *indev;

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
    rotaryEncoder.setBoundaries(-1000, 1000, false);
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

void set_lv_drv()
{
    /*Initialize lvgl*/
    lv_init();

#if LV_USE_LOG != 0
    lv_log_register_print_cb(my_log_cb);
#endif

    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t buf[screenWidth * 10];
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
