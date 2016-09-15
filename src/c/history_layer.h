#pragma once

#define HISTORY_LAYER_HEIGHT 14
#define HISTORY_LAYER_COMPONENT_WIDTH 10
#define HISTORY_LAYER_COMPONENT_SPACING 4

typedef enum {
  POINT,
  SPACE
} ComponentType;

typedef Layer HistoryLayer;

typedef struct {
  ComponentType configuration[50];
  uint8_t component_count;
  GColor8 color;
} HistoryLayerState;

HistoryLayer* history_layer_create(GRect, GColor8);
void history_layer_destroy(HistoryLayer*);
void history_layer_set_color(HistoryLayer*, GColor8);
void history_layer_push_component(HistoryLayer*, ComponentType);
void history_layer_pop_component(HistoryLayer*);
