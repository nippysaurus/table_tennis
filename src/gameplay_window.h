#pragma once

#include "table_tennis.h"

typedef void (*game_over_callback)();

void gameplay_window_create();
void gameplay_window_destroy();