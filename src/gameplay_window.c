#include <pebble.h>
#include "gameplay_window.h"

static Window *s_main_window;

// action bar
static ActionBarLayer *s_main_window_action_bar;
static GBitmap *s_bitmap_button_increment_score;
static GBitmap *s_bitmap_button_more;

static GameSummary *s_game_summary;

static void increment_top_player() {
  game_summary_layer_increment_top_player(s_game_summary);
}

static void increment_bottom_player() {
  game_summary_layer_increment_bottom_player(s_game_summary);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler) increment_top_player);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler) increment_bottom_player);
}

static void create_action_bar(Window *window) {
  // load icons assets
  s_bitmap_button_increment_score = gbitmap_create_with_resource(RESOURCE_ID_BUTTON_INCREMENT_SCORE);
  s_bitmap_button_more = gbitmap_create_with_resource(RESOURCE_ID_BUTTON_MORE);

  // create action bar
  s_main_window_action_bar = action_bar_layer_create();
  
  // set icons
  action_bar_layer_set_icon(s_main_window_action_bar, BUTTON_ID_UP, s_bitmap_button_increment_score);
  action_bar_layer_set_icon(s_main_window_action_bar, BUTTON_ID_SELECT, s_bitmap_button_more);
  action_bar_layer_set_icon(s_main_window_action_bar, BUTTON_ID_DOWN, s_bitmap_button_increment_score);
  
  action_bar_layer_set_click_config_provider(s_main_window_action_bar, click_config_provider);
  
  action_bar_layer_add_to_window(s_main_window_action_bar, window);
}

static void destroy_action_bar() {
  // destroy icons
  gbitmap_destroy(s_bitmap_button_increment_score);
  gbitmap_destroy(s_bitmap_button_more);
  
  action_bar_layer_destroy(s_main_window_action_bar);
}

static void create_example_game_summary(Window *window) {

  
  // Get the bounds of the Window
  struct Layer* window_layer = window_get_root_layer(s_main_window);
  GRect window_bounds = layer_get_bounds(window_layer);
  
  s_game_summary = game_summary_layer_create(GRect(0,0,window_bounds.size.w - ACTION_BAR_WIDTH, window_bounds.size.h));

  layer_add_child(
    window_get_root_layer(s_main_window),
    game_summary_layer_get_layer(s_game_summary)
  );
}

static void destroy_example_game_summary() {
  game_summary_layer_destroy(s_game_summary);
}

void setup_scoring_window() {
  s_main_window = window_create();
  
  create_action_bar(s_main_window);
  create_example_game_summary(s_main_window);

  window_stack_push(s_main_window, false);
}

void teardown_scoring_window() {
  window_stack_remove(s_main_window, false);
  
  destroy_example_game_summary();
  destroy_action_bar();
  
  window_destroy(s_main_window);
}