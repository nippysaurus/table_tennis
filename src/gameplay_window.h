#pragma once

#include "table_tennis.h"

typedef void (*game_over_callback)(TeamNumber winning_team);

void gameplay_window_create(GameLength game_length, TeamNumber first_serve, game_over_callback game_over_callback);
void gameplay_window_destroy();
