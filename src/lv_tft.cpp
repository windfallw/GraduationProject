#include "lv_tft.h"

#define ROTARY_ENCODER_BUTTON_PIN 25
#define ROTARY_ENCODER_A_PIN 26
#define ROTARY_ENCODER_B_PIN 27
#define ROTARY_ENCODER_VCC_PIN -1
#define ROTARY_ENCODER_STEPS 4

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);

TFT_eSPI tft = TFT_eSPI();

// TFT_ESPI reverse height and width in lvgl
const uint32_t screenWidth = TFT_HEIGHT;
const uint32_t screenHeight = TFT_WIDTH;

lv_disp_draw_buf_t draw_buf;
lv_color_t buf[screenWidth * 10];

lv_disp_t *disp;
lv_indev_t *indev;

lv_obj_t *cz_label;
lv_obj_t *wifi_label;

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
    lv_init();

#if LV_USE_LOG != 0
    lv_log_register_print_cb(my_log_cb);
#endif

    tft.begin();
    tft.setRotation(1);

    lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);

    /*Initialize the display*/
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

void event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        LV_LOG_USER("Clicked");
    }
    else if (code == LV_EVENT_VALUE_CHANGED)
    {
        LV_LOG_USER("Toggled");
    }
}

void set_ui()
{
    /* Ui design */
    lv_group_t *g = lv_group_create();
    lv_indev_set_group(indev, g);

    lv_obj_t *label;

    lv_obj_t *btn1 = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -40);

    label = lv_label_create(btn1);
    lv_label_set_text(label, "Button");
    lv_obj_center(label);

    lv_obj_t *btn2 = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn2, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 40);
    lv_obj_add_flag(btn2, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_height(btn2, LV_SIZE_CONTENT);

    label = lv_label_create(btn2);
    lv_label_set_text(label, "Toggle");
    lv_obj_center(label);

    cz_label = lv_label_create(lv_scr_act());
    lv_obj_align(cz_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(cz_label, &lv_font_simsun_16_cjk, 0);
    lv_label_set_text(cz_label, "当前:  mm");

    wifi_label = lv_label_create(lv_scr_act());
    lv_obj_align(wifi_label, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_set_style_text_font(wifi_label, &lv_font_montserrat_20, 0);
    lv_label_set_text(wifi_label, LV_SYMBOL_WIFI);
    lv_group_add_obj(g, btn1);
    lv_group_add_obj(g, btn2);
}