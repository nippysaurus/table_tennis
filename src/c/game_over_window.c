#include <pebble.h>
#include "game_over_window.h"

static Window *s_game_over_window;
static TextLayer *s_score_text_layer;
static Layer *s_icon_layer;
static GDrawCommandImage *s_icon_image;

static char s_final_score[7];

static void update_proc(Layer *layer, GContext *ctx) {
  // Place image in the center of the Window
  GSize img_size = gdraw_command_image_get_bounds_size(s_icon_image);
  GRect bounds = layer_get_bounds(layer);

  const GEdgeInsets frame_insets = {
    .top = (bounds.size.h - img_size.h) / 2,
    .left = (bounds.size.w - img_size.w) / 2
  };

  gdraw_command_image_draw(ctx, s_icon_image, grect_inset(bounds, frame_insets).origin);
}

void game_over_window_create(bool won, uint8_t team_1_score, uint8_t team_2_score) {
  s_game_over_window = window_create();

  // Get the bounds of the Window
  Layer* window_layer = window_get_root_layer(s_game_over_window);
  GRect window_layer_bounds = layer_get_bounds(window_layer);

  s_score_text_layer = text_layer_create(GRect(10, 110, window_layer_bounds.size.w - 20, 30));
  text_layer_set_overflow_mode(s_score_text_layer, GTextOverflowModeWordWrap);
  text_layer_set_font(s_score_text_layer, fonts_get_system_font(FONT_KEY_LECO_26_BOLD_NUMBERS_AM_PM));
  text_layer_set_text_alignment(s_score_text_layer, GTextAlignmentCenter);

  snprintf(s_final_score, 7, "%d:%d", team_1_score, team_2_score);

  GColor color_winner = PBL_IF_COLOR_ELSE(GColorGreen,GColorWhite);
  GColor color_loser = PBL_IF_COLOR_ELSE(GColorRed,GColorWhite);

  if (won) {
    window_set_background_color(s_game_over_window, color_winner);
    text_layer_set_text(s_score_text_layer, s_final_score);
    text_layer_set_background_color(s_score_text_layer, color_winner);
    s_icon_image = gdraw_command_image_create_with_resource(RESOURCE_ID_WINNER_ICON);
  } else {
    window_set_background_color(s_game_over_window, color_loser);
    text_layer_set_text(s_score_text_layer, s_final_score);
    text_layer_set_background_color(s_score_text_layer, color_loser);
    s_icon_image = gdraw_command_image_create_with_resource(RESOURCE_ID_LOSER_ICON);
  }

  // Create canvas Layer and set up the update procedure
  s_icon_layer = layer_create(GRect(0, 10, window_layer_bounds.size.w, 100));
  layer_set_update_proc(s_icon_layer, update_proc);
  layer_add_child(
    window_get_root_layer(s_game_over_window),
    s_icon_layer
  );

  layer_add_child(
    window_get_root_layer(s_game_over_window),
    text_layer_get_layer(s_score_text_layer)
  );

  window_stack_push(s_game_over_window, false);
}

void game_over_window_destroy() {
  window_stack_remove(s_game_over_window, false);
  text_layer_destroy(s_score_text_layer);
  layer_destroy(s_icon_layer);
  gdraw_command_image_destroy(s_icon_image);
  window_destroy(s_game_over_window);
}
