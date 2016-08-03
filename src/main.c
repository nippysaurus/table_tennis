#include <pebble.h>
#include "gameplay_window.h"
#include "game_over_window.h"

bool gameplay_window_visible = false;
bool game_over_window_visible = false;

static void game_over() {
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "game over");
  if (gameplay_window_visible == true) {
    gameplay_window_destroy();
    gameplay_window_visible = false;
  }
  game_over_window_create(true);
}

static void init() {
  gameplay_window_create(game_over);
  gameplay_window_visible = true;
}

static void deinit() {
  if (gameplay_window_visible == true) {
    gameplay_window_destroy();
    gameplay_window_visible = false;
  }
}

int main() {
  init();
  app_event_loop();
  deinit();
}