#pragma once

#include "table_tennis.h"
#include "game_setup_window.h"
#include "history_layer.h"

typedef void (*game_over_callback)(TeamNumber winning_team, uint8_t team_1_score, uint8_t team_2_score);

void gameplay_window_create(GameSetup game_setup, game_over_callback game_over_callback);
void gameplay_window_destroy();
