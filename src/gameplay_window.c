#include <pebble.h>
#include "gameplay_window.h"

#define SCORE_TEXT_LAYER_HEIGHT 55

static Window *s_main_window;

// action bar
static ActionBarLayer *s_main_window_action_bar;
static GBitmap *s_bitmap_button_increment_score;
static GBitmap *s_bitmap_button_more;

// game summary layers
Layer *game_summary_layer;
TextLayer *text_layer_t_bg;
TextLayer *text_layer_t;
TextLayer *text_layer_b_bg;
TextLayer *text_layer_b;

// game state
TableTennis* game_state;

static void game_summary_layer_display_updated_state();

static game_over_callback game_over;

static void increment_top_player() {
  table_tennis_increment_score(game_state, TEAM_1);
  game_summary_layer_display_updated_state();
}

static void increment_bottom_player() {
  table_tennis_increment_score(game_state, TEAM_2);
  game_summary_layer_display_updated_state();
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler) increment_top_player);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler) increment_bottom_player);
}



static void game_summary_layer_display_updated_state() {
  // handle game over logic
  if (game_state->winner != NO_TEAM) {
    game_over(game_state->winner);
    return;
  }
  
  GColor8 top_player_bg;
  GColor8 top_player_fg;
  GColor8 bottom_player_bg;
  GColor8 bottom_player_fg;
  if (game_state->teams[TEAM_1].serving == true) {
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
  text_layer_set_background_color(text_layer_t_bg, top_player_bg);
  text_layer_set_text_color(text_layer_t, top_player_fg);
  text_layer_set_background_color(text_layer_t, top_player_bg);
  text_layer_set_text(text_layer_t, game_state->teams[TEAM_1].score_formatted);
  // bottom player
  text_layer_set_background_color(text_layer_b_bg, bottom_player_bg);
  text_layer_set_text_color(text_layer_b, bottom_player_fg);
  text_layer_set_background_color(text_layer_b, bottom_player_bg);
  text_layer_set_text(text_layer_b, game_state->teams[TEAM_2].score_formatted);
}


void gameplay_window_create(game_over_callback game_over_callback) {
  game_over = game_over_callback;

  s_main_window = window_create();

  // create action bar
  s_main_window_action_bar = action_bar_layer_create();
  
  // load action bar icons assets
  s_bitmap_button_increment_score = gbitmap_create_with_resource(RESOURCE_ID_BUTTON_INCREMENT_SCORE);
  s_bitmap_button_more = gbitmap_create_with_resource(RESOURCE_ID_BUTTON_MORE);
  
  // set action bar icons
  action_bar_layer_set_icon(s_main_window_action_bar, BUTTON_ID_UP, s_bitmap_button_increment_score);
  action_bar_layer_set_icon(s_main_window_action_bar, BUTTON_ID_SELECT, s_bitmap_button_more);
  action_bar_layer_set_icon(s_main_window_action_bar, BUTTON_ID_DOWN, s_bitmap_button_increment_score);
  
  // set action bar click provider
  action_bar_layer_set_click_config_provider(s_main_window_action_bar, click_config_provider);
  
  // add action bar to window
  action_bar_layer_add_to_window(s_main_window_action_bar, s_main_window);
  
  

  

  // Get the bounds of the Window
  Layer* window_layer = window_get_root_layer(s_main_window);
  GRect window_bounds = layer_get_bounds(window_layer);
  
  GRect frame = GRect(
    0,
    0,
    window_bounds.size.w - ACTION_BAR_WIDTH,
    window_bounds.size.h
  );

  //char str[80];
  //snprintf(str, 80, "%d", frame.size.w);
  //APP_LOG(APP_LOG_LEVEL_DEBUG, str);
  //snprintf(str, 80, "%d", frame.size.h);
  //APP_LOG(APP_LOG_LEVEL_DEBUG, str);

  //text_layer_t_bg = text_layer_create(GRect(0, 0, frame.size.w, frame.size.h));
  //text_layer_set_background_color(text_layer_t_bg, GColorBlack);
  //layer_add_child(
  //  window_get_root_layer(s_main_window),
  //  text_layer_get_layer(text_layer_t_bg)
  //);
  
  game_state = table_tennis_create(SHORT_GAME, TEAM_1);
  
  game_summary_layer = layer_create(GRect(0, 0, frame.size.w, frame.size.h));
  
  // top background
  text_layer_t_bg = text_layer_create(GRect(0, 0, frame.size.w, frame.size.h / 2));
  layer_add_child(game_summary_layer, text_layer_get_layer(text_layer_t_bg));
  
  // top text
  text_layer_t = text_layer_create(GRect(10, 10, frame.size.w - 20, SCORE_TEXT_LAYER_HEIGHT));
  text_layer_set_overflow_mode(text_layer_t, GTextOverflowModeWordWrap);
  text_layer_set_text_alignment(text_layer_t, GTextAlignmentCenter);
  text_layer_set_font(text_layer_t, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  layer_add_child(game_summary_layer, text_layer_get_layer(text_layer_t));
  
  // bottom background
  text_layer_b_bg = text_layer_create(GRect(0, frame.size.h / 2, frame.size.w, frame.size.h / 2));
  layer_add_child(game_summary_layer, text_layer_get_layer(text_layer_b_bg));
  
  // bottom text
  text_layer_b = text_layer_create(GRect(10, (frame.size.h - SCORE_TEXT_LAYER_HEIGHT) - 10, frame.size.w - 20, SCORE_TEXT_LAYER_HEIGHT));
  text_layer_set_overflow_mode(text_layer_b, GTextOverflowModeWordWrap);
  text_layer_set_text_alignment(text_layer_b, GTextAlignmentCenter);
  text_layer_set_font(text_layer_b, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  layer_add_child(game_summary_layer, text_layer_get_layer(text_layer_b));

  game_summary_layer_display_updated_state();
  
  // add game summary layer to window
  layer_add_child(
    window_get_root_layer(s_main_window),
    game_summary_layer
  );
  
  
  

  window_stack_push(s_main_window, false);  
}

void gameplay_window_destroy() {
  window_stack_remove(s_main_window, false);
  
  // destroy action bar icons
  gbitmap_destroy(s_bitmap_button_increment_score);
  gbitmap_destroy(s_bitmap_button_more);
  
  // destroy action bar later
  action_bar_layer_destroy(s_main_window_action_bar);
  
  
  
  text_layer_destroy(text_layer_t);
  text_layer_destroy(text_layer_t_bg);
  text_layer_destroy(text_layer_b);
  text_layer_destroy(text_layer_b_bg);
  layer_destroy(game_summary_layer);
  
  table_tennis_destroy(game_state);
  
  window_destroy(s_main_window);
}