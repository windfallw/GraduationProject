#include "lvgl.h"
#include "app_hal.h"
#include "lvtft.h"

int main(void)
{
  lv_init();
  hal_setup();

  set_ui();

  lv_label_set_text(wifi_label, "WiFi");
  lv_label_set_text(battery_ico, LV_SYMBOL_BATTERY_FULL);
  lv_obj_clear_flag(battery_cg_ico, LV_OBJ_FLAG_HIDDEN);

  while (1)
  {
    for (int i = 0; i <= 100; i++)
    {
      lv_label_set_text_fmt(battery_label, "%d%%", i);
      lv_obj_align_all();
      lv_task_handler();
    }
  }
}
