#pragma once

#include "table_tennis.h"
#include "game_setup_window.h"

typedef void (*game_over_callback)(TeamNumber winning_team);

void gameplay_window_create(GameSetup game_setup, game_over_callback game_over_callback);
void gameplay_window_destroy();
