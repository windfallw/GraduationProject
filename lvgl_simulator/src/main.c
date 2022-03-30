#include <SDL2/SDL.h>

#include "lvgl.h"
#include "app_hal.h"
#include "lvtft.h"

int main(void)
{
  lv_init();
  hal_setup();

  set_lvgl();

  lv_scr_load(main_screen);
  lv_scr_load_anim(charge_screen, LV_SCR_LOAD_ANIM_FADE_ON, 100, 50, false);

  lv_label_set_text(wifi_txt, "STA ESPSTA | AP ESPAP");
  lv_label_set_text(battery_level_ico, LV_SYMBOL_BATTERY_FULL);

  lv_obj_clear_flag(battery_lightning_ico, LV_OBJ_FLAG_HIDDEN);

  while (1)
  {
    // lv_task_handler();
    // SDL_Delay(5);

    for (int i = 0; i <= 100; i++)
    {
      lv_label_set_text_fmt(battery_level_txt, "%d%%", i);
      align_lv_top_status_bar();

      lv_task_handler();
      SDL_Delay(1);
    }
  }
}
