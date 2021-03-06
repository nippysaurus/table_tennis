#include <pebble.h>
#include "gameplay_window.h"
#include "history_layer.h"

#define SCORE_TEXT_LAYER_HEIGHT 55

static Window *s_main_window;

// action bar
static ActionBarLayer *s_main_window_action_bar;
static GBitmap *s_bitmap_button_increment_score;
static GBitmap *s_bitmap_button_undo;

// game summary layers
static Layer *s_game_summary_layer;
static TextLayer *s_text_layer_t_bg;
static TextLayer *s_text_layer_t;
static HistoryLayer *s_history_layer_t;
static TextLayer *s_text_layer_b_bg;
static TextLayer *s_text_layer_b;
static HistoryLayer *s_history_layer_b;

// game state
static TableTennis* s_game_state;

static GameSetup s_game_setup;

static void s_game_summary_layer_display_updated_state();

static GameOverCallback s_game_over;

static void increment_top_player() {
  table_tennis_increment_score(s_game_state, TEAM_1);
  history_layer_push_component(s_history_layer_t, POINT);
  history_layer_push_component(s_history_layer_b, SPACE);
  s_game_summary_layer_display_updated_state();
}

static void increment_bottom_player() {
  table_tennis_increment_score(s_game_state, TEAM_2);
  history_layer_push_component(s_history_layer_t, SPACE);
  history_layer_push_component(s_history_layer_b, POINT);
  s_game_summary_layer_display_updated_state();
}

static void undo() {
  if (table_tennis_can_undo(s_game_state) == true) {
    table_tennis_undo_score(s_game_state);
    history_layer_pop_component(s_history_layer_t);
    history_layer_pop_component(s_history_layer_b);
    s_game_summary_layer_display_updated_state();
  }
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler) increment_top_player);
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler) undo);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler) increment_bottom_player);
}

static void s_game_summary_layer_display_updated_state() {
  // handle game over logic
  if (s_game_state->winner != NO_TEAM) {
    s_game_over(
      s_game_state->winner,
      s_game_state->teams[TEAM_1].score,
      s_game_state->teams[TEAM_2].score
    );
    if (s_game_setup.game_over == SINGLE) {
      vibes_short_pulse();
    }
    if (s_game_setup.game_over == DOUBLE) {
      vibes_double_pulse();
    }
    return;
  }

  if (s_game_state->serve_just_changed) {
    if (s_game_setup.serve_change == SINGLE) {
      vibes_short_pulse();
    }
    if (s_game_setup.serve_change == DOUBLE) {
      vibes_double_pulse();
    }
  }

  GColor8 top_player_bg;
  GColor8 top_player_fg;
  GColor8 bottom_player_bg;
  GColor8 bottom_player_fg;
  if (s_game_state->teams[TEAM_1].serving == true) {
    #if defined(PBL_BW)
    top_player_bg = GColorLightGray;
    top_player_fg = GColorBlack;
    bottom_player_bg = GColorWhite;
    bottom_player_fg = GColorBlack;
    #elif defined(PBL_COLOR)
    top_player_bg = GColorBlue;
    top_player_fg = GColorWhite;
    bottom_player_bg = GColorWhite;
    bottom_player_fg = GColorBlue;
    #endif
  } else {
    #if defined(PBL_BW)
    top_player_bg = GColorWhite;
    top_player_fg = GColorBlack;
    bottom_player_bg = GColorLightGray;
    bottom_player_fg = GColorBlack;
    #elif defined(PBL_COLOR)
    top_player_bg = GColorWhite;
    top_player_fg = GColorBlue;
    bottom_player_bg = GColorBlue;
    bottom_player_fg = GColorWhite;
    #endif

    if (s_game_setup.player_serve == SINGLE) {
      vibes_short_pulse();
    }
    if (s_game_setup.player_serve == DOUBLE) {
      vibes_double_pulse();
    }

  }
  // top player
  text_layer_set_background_color(s_text_layer_t_bg, top_player_bg);
  text_layer_set_text_color(s_text_layer_t, top_player_fg);
  text_layer_set_background_color(s_text_layer_t, top_player_bg);
  text_layer_set_text(s_text_layer_t, s_game_state->teams[TEAM_1].score_formatted);
  history_layer_set_color(s_history_layer_t, top_player_fg);
  // bottom player
  text_layer_set_background_color(s_text_layer_b_bg, bottom_player_bg);
  text_layer_set_text_color(s_text_layer_b, bottom_player_fg);
  text_layer_set_background_color(s_text_layer_b, bottom_player_bg);
  text_layer_set_text(s_text_layer_b, s_game_state->teams[TEAM_2].score_formatted);
  history_layer_set_color(s_history_layer_b, bottom_player_fg);
}


void gameplay_window_create(GameSetup game_setup, GameOverCallback GameOverCallback) {
  s_game_over = GameOverCallback;

  s_game_setup = game_setup;

  s_main_window = window_create();

  //
  // create game summary
  //

  // Get the bounds of the Window
  Layer* window_layer = window_get_root_layer(s_main_window);
  GRect window_bounds = layer_get_bounds(window_layer);

  GRect frame = GRect(
    0,
    0,
    window_bounds.size.w,
    window_bounds.size.h
  );

  s_game_state = table_tennis_create(
    s_game_setup.game_length,
    s_game_setup.first_serve
  );

  s_game_summary_layer = layer_create(GRect(0, 0, frame.size.w, frame.size.h));

  // top background
  s_text_layer_t_bg = text_layer_create(GRect(0, 0, frame.size.w, frame.size.h / 2));
  layer_add_child(s_game_summary_layer, text_layer_get_layer(s_text_layer_t_bg));

  // top text
  s_text_layer_t = text_layer_create(GRect(10, 10, frame.size.w - PBL_IF_RECT_ELSE(ACTION_BAR_WIDTH, 0) - 20, SCORE_TEXT_LAYER_HEIGHT));
  text_layer_set_overflow_mode(s_text_layer_t, GTextOverflowModeWordWrap);
  text_layer_set_text_alignment(s_text_layer_t, GTextAlignmentCenter);
  text_layer_set_font(s_text_layer_t, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS));
  layer_add_child(s_game_summary_layer, text_layer_get_layer(s_text_layer_t));

  // top history
  s_history_layer_t = history_layer_create(
    GRect(
      0,
      (frame.size.h / 2) - HISTORY_LAYER_HEIGHT,
      frame.size.w - ACTION_BAR_WIDTH,
      HISTORY_LAYER_HEIGHT
    ),
    GColorBlack
  );
  layer_add_child(s_game_summary_layer, s_history_layer_t);

  // bottom background
  s_text_layer_b_bg = text_layer_create(GRect(0, frame.size.h / 2, frame.size.w, frame.size.h / 2));
  layer_add_child(s_game_summary_layer, text_layer_get_layer(s_text_layer_b_bg));

  // bottom text
  s_text_layer_b = text_layer_create(GRect(10, (frame.size.h - SCORE_TEXT_LAYER_HEIGHT) - 10, frame.size.w - PBL_IF_RECT_ELSE(ACTION_BAR_WIDTH, 0) - 20, SCORE_TEXT_LAYER_HEIGHT));
  text_layer_set_overflow_mode(s_text_layer_b, GTextOverflowModeWordWrap);
  text_layer_set_text_alignment(s_text_layer_b, GTextAlignmentCenter);
  text_layer_set_font(s_text_layer_b, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS));
  layer_add_child(s_game_summary_layer, text_layer_get_layer(s_text_layer_b));

  // bottom history
  s_history_layer_b = history_layer_create(
    GRect(
      0,
      frame.size.h / 2,
      frame.size.w - ACTION_BAR_WIDTH,
      HISTORY_LAYER_HEIGHT
    ),
    GColorBlack
  );
  layer_add_child(s_game_summary_layer, s_history_layer_b);

  s_game_summary_layer_display_updated_state();

  // add game summary layer to window
  layer_add_child(
    window_get_root_layer(s_main_window),
    s_game_summary_layer
  );

  // 
  // create action bar
  //

  s_main_window_action_bar = action_bar_layer_create();

  // load action bar icons assets
  s_bitmap_button_increment_score = gbitmap_create_with_resource(RESOURCE_ID_BUTTON_INCREMENT_SCORE);
  s_bitmap_button_undo = gbitmap_create_with_resource(RESOURCE_ID_BUTTON_UNDO);

  // set action bar icons
  action_bar_layer_set_icon(s_main_window_action_bar, BUTTON_ID_UP, s_bitmap_button_increment_score);
  // just hiding this action bar icon until it is actually implemented
  action_bar_layer_set_icon(s_main_window_action_bar, BUTTON_ID_SELECT, s_bitmap_button_undo);
  action_bar_layer_set_icon(s_main_window_action_bar, BUTTON_ID_DOWN, s_bitmap_button_increment_score);

  // set action bar click provider
  action_bar_layer_set_click_config_provider(s_main_window_action_bar, click_config_provider);

  // add action bar to window
  action_bar_layer_add_to_window(s_main_window_action_bar, s_main_window);

  window_stack_push(s_main_window, false);
}

void gameplay_window_destroy() {
  window_stack_remove(s_main_window, false);

  // destroy action bar icons
  gbitmap_destroy(s_bitmap_button_increment_score);
  gbitmap_destroy(s_bitmap_button_undo);

  // destroy action bar later
  action_bar_layer_destroy(s_main_window_action_bar);

  text_layer_destroy(s_text_layer_t);
  text_layer_destroy(s_text_layer_t_bg);
  history_layer_destroy(s_history_layer_t);
  text_layer_destroy(s_text_layer_b);
  text_layer_destroy(s_text_layer_b_bg);
  history_layer_destroy(s_history_layer_b);
  layer_destroy(s_game_summary_layer);

  table_tennis_destroy(s_game_state);

  window_destroy(s_main_window);
}
