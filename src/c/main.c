#include <pebble.h>
#include "game_setup_window.h"
#include "gameplay_window.h"
#include "game_over_window.h"

static bool s_game_setup_window_visible = false;
static bool s_gameplay_window_visible = false;
static bool s_game_over_window_visible = false;

static void game_over(TeamNumber winning_team, uint8_t team_1_score, uint8_t team_2_score) {
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "game over");
  if (s_gameplay_window_visible == true) {
    gameplay_window_destroy();
    s_gameplay_window_visible = false;
  }
  game_over_window_create(
    winning_team == TEAM_2,
    team_1_score,
    team_2_score
  );
  s_game_over_window_visible = true;
}

static void game_setup_ready_callback(GameSetup game_setup) {
  if (s_game_setup_window_visible == true) {
    game_setup_window_destroy();
    s_game_setup_window_visible = false;
  }
  gameplay_window_create(
    game_setup,
    game_over
  );
  s_gameplay_window_visible = true;
}

static void init() {
  game_setup_window_create(game_setup_ready_callback);
  s_game_setup_window_visible = true;
}

static void deinit() {
  if (s_game_setup_window_visible == true) {
    game_setup_window_destroy();
    s_game_setup_window_visible = false;
  }
  if (s_gameplay_window_visible == true) {
    gameplay_window_destroy();
    s_gameplay_window_visible = false;
  }
  if (s_game_over_window_visible == true) {
    game_over_window_destroy();
    s_game_over_window_visible = false;
  }
}

int main() {
  init();
  app_event_loop();
  deinit();
}
