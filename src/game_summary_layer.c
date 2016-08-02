#include <pebble.h>
#include "game_summary_layer.h"

#define SCORE_TEXT_LAYER_HEIGHT 55

static void game_summary_layer_display_updated_state(GameSummary* game_summary);

GameSummary* game_summary_layer_create(GRect frame) {
  GameSummary* game_summary = malloc(sizeof(GameSummary));
  game_summary->game_state = table_tennis_create(SHORT_GAME, TEAM_1);
  game_summary->game_summary_layer = layer_create(GRect(0, 0, frame.size.w, frame.size.h));
  
  // top background
  game_summary->text_layer_t_bg = text_layer_create(GRect(0, 0, frame.size.w, frame.size.h / 2));
  layer_add_child(game_summary->game_summary_layer, text_layer_get_layer(game_summary->text_layer_t_bg));
  
  // top text
  game_summary->text_layer_t = text_layer_create(GRect(10, 10, frame.size.w - 20, SCORE_TEXT_LAYER_HEIGHT));
  text_layer_set_overflow_mode(game_summary->text_layer_t, GTextOverflowModeWordWrap);
  text_layer_set_text_alignment(game_summary->text_layer_t, GTextAlignmentCenter);
  text_layer_set_font(game_summary->text_layer_t, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  layer_add_child(game_summary->game_summary_layer, text_layer_get_layer(game_summary->text_layer_t));
  
  // bottom background
  game_summary->text_layer_b_bg = text_layer_create(GRect(0, frame.size.h / 2, frame.size.w, frame.size.h / 2));
  layer_add_child(game_summary->game_summary_layer, text_layer_get_layer(game_summary->text_layer_b_bg));
  
  // bottom text
  game_summary->text_layer_b = text_layer_create(GRect(10, (frame.size.h - SCORE_TEXT_LAYER_HEIGHT) - 10, frame.size.w - 20, SCORE_TEXT_LAYER_HEIGHT));
  text_layer_set_overflow_mode(game_summary->text_layer_b, GTextOverflowModeWordWrap);
  text_layer_set_text_alignment(game_summary->text_layer_b, GTextAlignmentCenter);
  text_layer_set_font(game_summary->text_layer_b, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  layer_add_child(game_summary->game_summary_layer, text_layer_get_layer(game_summary->text_layer_b));
  
  game_summary_layer_display_updated_state(game_summary);
  
  return game_summary;
}

Layer* game_summary_layer_get_layer(GameSummary* game_summary) {
  return game_summary->game_summary_layer;
}

void game_summary_layer_destroy(GameSummary* game_summary) {
  text_layer_destroy(game_summary->text_layer_t);
  text_layer_destroy(game_summary->text_layer_t_bg);
  text_layer_destroy(game_summary->text_layer_b);
  text_layer_destroy(game_summary->text_layer_b_bg);
  layer_destroy(game_summary->game_summary_layer);
  table_tennis_destroy(game_summary->game_state);
}

static void game_summary_layer_display_updated_state(GameSummary* game_summary) {
  GColor8 top_player_bg;
  GColor8 top_player_fg;
  GColor8 bottom_player_bg;
  GColor8 bottom_player_fg;
  if (game_summary->game_state->teams[TEAM_1].serving == true) {
    #if defined(PBL_BW)
    top_player_bg = GColorLightGray;
    top_player_fg = GColorBlack;
    bottom_player_bg = GColorWhite;
    bottom_player_fg = GColorBlack;
    #elif defined(PBL_COLOR)
    top_player_bg = GColorMediumAquamarine;
    top_player_fg = GColorBlack;
    bottom_player_bg = GColorWhite;
    bottom_player_fg = GColorBlack;
    #endif
  } else {
    #if defined(PBL_BW)
    top_player_bg = GColorWhite;
    top_player_fg = GColorBlack;
    bottom_player_bg = GColorLightGray;
    bottom_player_fg = GColorBlack;
    #elif defined(PBL_COLOR)
    top_player_bg = GColorWhite;
    top_player_fg = GColorBlack;
    bottom_player_bg = GColorMediumAquamarine;
    bottom_player_fg = GColorBlack;
    #endif
  }
  // top player
  text_layer_set_background_color(game_summary->text_layer_t_bg, top_player_bg);
  text_layer_set_text_color(game_summary->text_layer_t, top_player_fg);
  text_layer_set_background_color(game_summary->text_layer_t, top_player_bg);
  text_layer_set_text(game_summary->text_layer_t, game_summary->game_state->teams[TEAM_1].score_formatted);
  // bottom player
  text_layer_set_background_color(game_summary->text_layer_b_bg, bottom_player_bg);
  text_layer_set_text_color(game_summary->text_layer_b, bottom_player_fg);
  text_layer_set_background_color(game_summary->text_layer_b, bottom_player_bg);
  text_layer_set_text(game_summary->text_layer_b, game_summary->game_state->teams[TEAM_2].score_formatted);
}

void game_summary_layer_increment_top_player(GameSummary* game_summary) {
  table_tennis_increment_score(game_summary->game_state, TEAM_1);
  game_summary_layer_display_updated_state(game_summary);
}

void game_summary_layer_increment_bottom_player(GameSummary* game_summary) {
  table_tennis_increment_score(game_summary->game_state, TEAM_2);
  game_summary_layer_display_updated_state(game_summary);
}