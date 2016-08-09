#pragma once

#include "table_tennis.h"

typedef enum {
  NONE,
  SINGLE,
  DOUBLE
} VibrateOption;

typedef struct {
  GameLength game_length;
  TeamNumber first_serve;
  VibrateOption serve_change;
  VibrateOption game_over;
} GameSetup;

typedef void (*GameSetupReadyCallback)(GameSetup game_setup);

void game_setup_window_create(GameSetupReadyCallback game_setup_ready_callback);
void game_setup_window_destroy();
