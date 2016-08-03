#include <pebble.h>
#include "gameplay_window.h"

static void game_over() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "game over");
}

static void init() {
  gameplay_window_create(game_over);
}

static void deinit() {
  gameplay_window_destroy();
}

int main() {
  init();
  app_event_loop();
  deinit();
}