#pragma once

#include "table_tennis.h"

typedef struct {
  GameLength game_length;
  TeamNumber first_serve;
} GameSetup;

typedef void (*GameSetupReadyCallback)(GameSetup game_setup);

void game_setup_window_create(GameSetupReadyCallback game_setup_ready_callback);
void game_setup_window_destroy();
