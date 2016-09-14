#include <pebble.h>
#include "history_layer.h"

// ============================================================================

static void s_draw(Layer* layer, GContext *ctx) {
  HistoryLayerState *history_layer_state = layer_get_data(layer);

  graphics_context_set_fill_color(
    ctx,
    history_layer_state->color
  );

  GRect layer_bounds = layer_get_bounds(layer);

  for (int i = 0; i < history_layer_state->component_count; i++) {
    int inverse_index = history_layer_state->component_count - i - 1;
    bool is_point = history_layer_state->configuration[inverse_index] == POINT;
    if (is_point) {
      graphics_fill_rect(
        ctx,
        GRect(
          layer_bounds.size.w - (
            (i+1) * (
              HISTORY_LAYER_COMPONENT_WIDTH + HISTORY_LAYER_COMPONENT_SPACING
            )
          ),
          HISTORY_LAYER_COMPONENT_SPACING,
          HISTORY_LAYER_COMPONENT_WIDTH,
          HISTORY_LAYER_HEIGHT - (HISTORY_LAYER_COMPONENT_SPACING * 2)
        ),
        2,
        GCornersAll
      );
    }
  }
}

// ============================================================================

HistoryLayer* history_layer_create(GRect rect, GColor8 color) {
  HistoryLayer* history_layer = layer_create_with_data(
    rect,
    sizeof(HistoryLayerState)
  );

  HistoryLayerState *history_layer_state = layer_get_data(history_layer);

  history_layer_state->color = color;
  history_layer_state->component_count = 0;

  layer_set_update_proc(
    history_layer,
    &s_draw
  );

  return history_layer;
}

void history_layer_destroy(HistoryLayer* history_layer) {
  layer_destroy(history_layer);
}

void history_layer_push_component(HistoryLayer* history_layer, ComponentType component_type) {
  HistoryLayerState *history_layer_state = layer_get_data(history_layer);

  history_layer_state->configuration[history_layer_state->component_count] = component_type;
  history_layer_state->component_count++;

  layer_mark_dirty(history_layer);
}

void history_layer_pop_component(HistoryLayer* history_layer) {
  HistoryLayerState *history_layer_state = layer_get_data(history_layer);

  history_layer_state->component_count--;

  layer_mark_dirty(history_layer);
}

void history_layer_set_color(HistoryLayer* history_layer, GColor8 color) {
  HistoryLayerState *history_layer_state = layer_get_data(history_layer);

  history_layer_state->color = color;

  layer_mark_dirty(history_layer);
}
