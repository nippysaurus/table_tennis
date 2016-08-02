#pragma once

#include "table_tennis.h"

struct GameSummary {
  // game summary area
  Layer *game_summary_layer;
  // top player
  TextLayer *text_layer_t_bg;
  TextLayer *text_layer_t;
  // bottom player
  TextLayer *text_layer_b_bg;
  TextLayer *text_layer_b;
  // game state
  TableTennis* game_state;
};

typedef struct GameSummary GameSummary;

Layer* game_summary_layer_get_layer(GameSummary* game_summary);
GameSummary* game_summary_layer_create(GRect frame);
void game_summary_layer_destroy(GameSummary* game_summary);

void game_summary_layer_increment_top_player(GameSummary* game_summary);
void game_summary_layer_increment_bottom_player(GameSummary* game_summary);