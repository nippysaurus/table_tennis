#include <pebble.h>
#include "gameplay_window.h"

static void init() {
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "bla");
  setup_scoring_window();
}

static void deinit() {
  teardown_scoring_window();
}

int main() {
  init();
  app_event_loop();
  deinit();
}