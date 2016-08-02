#include <pebble.h>
#include "gameplay_window.h"

static void init() {
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "bla");
  gameplay_window_create();
}

static void deinit() {
  gameplay_window_destroy();
}

int main() {
  init();
  app_event_loop();
  deinit();
}