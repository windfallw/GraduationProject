#include "lvgl.h"
#include "app_hal.h"
#include "lvtft.h"

int main(void)
{
  lv_init();
  hal_setup();

  set_ui();

  hal_loop();
}
