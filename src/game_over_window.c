#include <pebble.h>
#include "game_over_window.h"

static Window *s_game_over_window;
static TextLayer *s_message_text_layer;

void game_over_window_create(bool won) {
  s_game_over_window = window_create();
  window_stack_push(s_game_over_window, false);
  
  // Get the bounds of the Window
  Layer* window_layer = window_get_root_layer(s_game_over_window);
  GRect window_bounds = layer_get_bounds(window_layer);
  
  s_message_text_layer = text_layer_create(GRect(
    10,
    10,
    window_bounds.size.w - 20,
    50
  ));
  text_layer_set_overflow_mode(s_message_text_layer, GTextOverflowModeWordWrap);
  text_layer_set_text_alignment(s_message_text_layer, GTextAlignmentCenter);
  if (won) {
    text_layer_set_text(s_message_text_layer, "YOU WIN");
  } else {
    text_layer_set_text(s_message_text_layer, "YOU LOSE");
  }

  layer_add_child(
    window_get_root_layer(s_game_over_window),
    text_layer_get_layer(s_message_text_layer)
  );
}

void game_over_window_destroy() {
  window_stack_remove(s_game_over_window, false);
  text_layer_destroy(s_message_text_layer);
  window_destroy(s_game_over_window);
}