#include <pebble.h>
#include "game_setup_window.h"
#include "gameplay_window.h"
#include "game_over_window.h"

bool game_setup_window_visible = false;
bool gameplay_window_visible = false;
bool game_over_window_visible = false;

static void game_over(TeamNumber winning_team, int team_1_score, int team_2_score) {
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "game over");
  if (gameplay_window_visible == true) {
    gameplay_window_destroy();
    gameplay_window_visible = false;
  }
  game_over_window_create(
    winning_team == TEAM_2,
    team_1_score,
    team_2_score
  );
  game_over_window_visible = true;
}

static void game_setup_ready_callback(GameSetup game_setup) {
  if (game_setup_window_visible == true) {
    game_setup_window_destroy();
    game_setup_window_visible = false;
  }
  gameplay_window_create(
    game_setup,
    game_over
  );
  gameplay_window_visible = true;
}

static void init() {
  game_setup_window_create(game_setup_ready_callback);
  game_setup_window_visible = true;
}

static void deinit() {
  if (game_setup_window_visible == true) {
    game_setup_window_destroy();
    game_setup_window_visible = false;
  }
  if (gameplay_window_visible == true) {
    gameplay_window_destroy();
    gameplay_window_visible = false;
  }
  if (game_over_window_visible == true) {
    game_over_window_destroy();
    game_over_window_visible = false;
  }
}

int main() {
  init();
  app_event_loop();
  deinit();
}
