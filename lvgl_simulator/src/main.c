/**
 ******************************************************************************
 * @file    main.c
 * @author  Ac6
 * @version V1.0
 * @date    01-December-2013
 * @brief   Default main function.
 ******************************************************************************
 */

/* main.c has been modified base on the version above */

#include <SDL2/SDL.h>
#include "app_hal.h"

#include "lvtft.h"

int main(void)
{
  lv_init();
  hal_setup();

  set_lvgl();

  lv_scr_load(main_screen);

  lv_label_set_text(top_bar->wifi_txt, "ESPSTA");
  lv_label_set_text(top_bar->level_ico, LV_SYMBOL_BATTERY_FULL);

  lv_obj_clear_flag(top_bar->lightning_ico, LV_OBJ_FLAG_HIDDEN);

  while (1)
  {
    // show_lv_charge_anim();

    for (int i = 0; i <= 100; i++)
    {
      lv_label_set_text_fmt(top_bar->level_txt, "%d%%", i);
      align_lv_top_status_bar();
      lv_task_handler();
      SDL_Delay(5);
    }
  }
}
