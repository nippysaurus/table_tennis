#pragma once

#include "table_tennis.h"
#include "game_setup_window.h"
#include "history_layer.h"

typedef void (*GameOverCallback)(TeamNumber winning_team, uint8_t team_1_score, uint8_t team_2_score);

void gameplay_window_create(GameSetup game_setup, GameOverCallback GameOverCallback);
void gameplay_window_destroy();
