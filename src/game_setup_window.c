#include <pebble.h>
#include "game_setup_window.h"

static Window *s_game_setup_window;
static MenuLayer *s_menu_layer;

static GameSetup s_game_setup;
static GameSetupReadyCallback s_game_setup_ready_callback;

#define NUM_MENU_SECTIONS 1
#define NUM_FIRST_MENU_ITEMS 7

typedef enum {
  CURRENTLY_SELECTED_GAME_LENGTH_INDEX,
  CURRENTLY_SELECTED_FIRST_SERVE_INDEX,
  CURRENTLY_SELECTED_PLAYER_SERVE_OPTION_INDEX,
  CURRENTLY_SELECTED_SERVE_CHANGE_OPTION_INDEX,
  CURRENTLY_SELECTED_GAME_OVER_OPTION_INDEX
} PersistanceKeys;

// game length state
int currently_selected_game_length_index;
GameLength game_length_options[2];
char* game_length_option_labels[2];

// first serve state
int currently_selected_first_serve_index;
TeamNumber first_serve_options[2];
char* first_serve_option_labels[2];

// player serve vibration
int currently_selected_player_serve_option_index;
VibrateOption player_serve_options[3];
char* player_serve_option_labels[3];

// serve change vibration
int currently_selected_serve_change_option_index;
VibrateOption serve_change_options[3];
char* serve_change_option_labels[3];

// game over vibration
int currently_selected_game_over_option_index;
VibrateOption game_over_options[3];
char* game_over_option_labels[3];

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      return NUM_FIRST_MENU_ITEMS;
    default:
      return 0;
  }
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      menu_cell_basic_header_draw(ctx, cell_layer, "Game Setup");
      break;
  }
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  switch (cell_index->section) {
    case 0:
      switch (cell_index->row) {
        case 0:
          {
            GSize size = layer_get_frame(cell_layer).size;
            graphics_draw_text(
              ctx,
              "START GAME",
              fonts_get_system_font(FONT_KEY_GOTHIC_28),
              GRect(0, 0, size.w, size.h),
              GTextOverflowModeTrailingEllipsis,
              GTextAlignmentCenter,
              NULL
            );
          }
          break;
        case 1:
          menu_cell_basic_draw(
            ctx,
            cell_layer,
            "Game Length",
            game_length_option_labels[currently_selected_game_length_index],
            NULL
          );
          break;
        case 2:
          menu_cell_basic_draw(
            ctx,
            cell_layer,
            "First Serve",
            first_serve_option_labels[currently_selected_first_serve_index],
            NULL
          );
          break;
        case 3:
          menu_cell_basic_draw(
            ctx,
            cell_layer,
            "Your Serve",
            player_serve_option_labels[currently_selected_player_serve_option_index],
            NULL
          );
          break;
        case 4:
          menu_cell_basic_draw(
            ctx,
            cell_layer,
            "Serve Change",
            serve_change_option_labels[currently_selected_serve_change_option_index],
            NULL
          );
          break;
        case 5:
          menu_cell_basic_draw(
            ctx,
            cell_layer,
            "Game Over",
            game_over_option_labels[currently_selected_game_over_option_index],
            NULL
          );
          break;
        case 6:
          {
            GSize size = layer_get_frame(cell_layer).size;
            graphics_draw_text(
              ctx,
              "START GAME",
              fonts_get_system_font(FONT_KEY_GOTHIC_28),
              GRect(0, 0, size.w, size.h),
              GTextOverflowModeTrailingEllipsis,
              GTextAlignmentCenter,
              NULL
            );
          }
          break;
      }
      break;
  }
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  switch (cell_index->row) {
    case 0:
      s_game_setup_ready_callback(s_game_setup);
      break;
    case 1:
      currently_selected_game_length_index++;
      if (currently_selected_game_length_index == sizeof(game_length_options)) {
        currently_selected_game_length_index = 0;
      }
      persist_write_int(
        CURRENTLY_SELECTED_GAME_LENGTH_INDEX,
        currently_selected_game_length_index
      );
      s_game_setup.game_length = game_length_options[currently_selected_game_length_index];
      layer_mark_dirty(menu_layer_get_layer(menu_layer));
      break;
    case 2:
      currently_selected_first_serve_index++;
      if (currently_selected_first_serve_index == sizeof(first_serve_options)) {
        currently_selected_first_serve_index = 0;
      }
      persist_write_int(
        CURRENTLY_SELECTED_FIRST_SERVE_INDEX,
        currently_selected_first_serve_index
      );
      s_game_setup.first_serve = first_serve_options[currently_selected_first_serve_index];
      layer_mark_dirty(menu_layer_get_layer(menu_layer));
      break;
    case 3:
      currently_selected_player_serve_option_index++;
      if (currently_selected_player_serve_option_index == sizeof(player_serve_options)) {
        currently_selected_player_serve_option_index = 0;
      }
      persist_write_int(
        CURRENTLY_SELECTED_PLAYER_SERVE_OPTION_INDEX,
        currently_selected_player_serve_option_index
      );
      s_game_setup.player_serve = player_serve_options[currently_selected_player_serve_option_index];
      layer_mark_dirty(menu_layer_get_layer(menu_layer));
      break;
    case 4:
      currently_selected_serve_change_option_index++;
      if (currently_selected_serve_change_option_index == sizeof(serve_change_options)) {
        currently_selected_serve_change_option_index = 0;
      }
      persist_write_int(
        CURRENTLY_SELECTED_SERVE_CHANGE_OPTION_INDEX,
        currently_selected_serve_change_option_index
      );
      s_game_setup.serve_change = serve_change_options[currently_selected_serve_change_option_index];
      layer_mark_dirty(menu_layer_get_layer(menu_layer));
      break;
    case 5:
      currently_selected_game_over_option_index++;
      if (currently_selected_game_over_option_index == sizeof(game_over_options)) {
        currently_selected_game_over_option_index = 0;
      }
      persist_write_int(
        CURRENTLY_SELECTED_GAME_OVER_OPTION_INDEX,
        currently_selected_game_over_option_index
      );
      s_game_setup.game_over = game_over_options[currently_selected_game_over_option_index];
      layer_mark_dirty(menu_layer_get_layer(menu_layer));
      break;
    case 6:
      s_game_setup_ready_callback(s_game_setup);
      break;
  }
}

void game_setup_window_create(GameSetupReadyCallback game_setup_ready_callback) {
  s_game_setup_ready_callback = game_setup_ready_callback;

  // game length state
  if (persist_exists(CURRENTLY_SELECTED_GAME_LENGTH_INDEX)) {
    currently_selected_game_length_index = persist_read_int(CURRENTLY_SELECTED_GAME_LENGTH_INDEX);
  } else {
    currently_selected_game_length_index = 0;
  }
  game_length_options[0] = SHORT_GAME;
  game_length_options[1] = LONG_GAME;
  game_length_option_labels[0] = "11";
  game_length_option_labels[1] = "21";

  s_game_setup.game_length = game_length_options[currently_selected_game_length_index];

  // first serve state
  if (persist_exists(CURRENTLY_SELECTED_FIRST_SERVE_INDEX)) {
    currently_selected_first_serve_index = persist_read_int(CURRENTLY_SELECTED_FIRST_SERVE_INDEX);
  } else {
    currently_selected_first_serve_index = 0;
  }
  first_serve_options[0] = TEAM_1;
  first_serve_options[1] = TEAM_2;
  first_serve_option_labels[0] = "Opponent";
  first_serve_option_labels[1] = "You";

  s_game_setup.first_serve = first_serve_options[currently_selected_first_serve_index];

  // serve change vibration
  if (persist_exists(CURRENTLY_SELECTED_PLAYER_SERVE_OPTION_INDEX)) {
    currently_selected_player_serve_option_index = persist_read_int(CURRENTLY_SELECTED_PLAYER_SERVE_OPTION_INDEX);
  } else {
    currently_selected_player_serve_option_index = 0;
  }
  player_serve_options[0] = NONE;
  player_serve_options[1] = SINGLE;
  player_serve_options[2] = DOUBLE;
  player_serve_option_labels[0] = "-";
  player_serve_option_labels[1] = "bzzt";
  player_serve_option_labels[2] = "bzzt bzzt";

  s_game_setup.player_serve = player_serve_options[currently_selected_player_serve_option_index];

  // serve change vibration
  if (persist_exists(CURRENTLY_SELECTED_SERVE_CHANGE_OPTION_INDEX)) {
    currently_selected_serve_change_option_index = persist_read_int(CURRENTLY_SELECTED_SERVE_CHANGE_OPTION_INDEX);
  } else {
    currently_selected_serve_change_option_index = 0;
  }
  serve_change_options[0] = NONE;
  serve_change_options[1] = SINGLE;
  serve_change_options[2] = DOUBLE;
  serve_change_option_labels[0] = "-";
  serve_change_option_labels[1] = "bzzt";
  serve_change_option_labels[2] = "bzzt bzzt";

  s_game_setup.serve_change = serve_change_options[currently_selected_serve_change_option_index];

  // game over vibration
  if (persist_exists(CURRENTLY_SELECTED_GAME_OVER_OPTION_INDEX)) {
    currently_selected_game_over_option_index = persist_read_int(CURRENTLY_SELECTED_GAME_OVER_OPTION_INDEX);
  } else {
    currently_selected_game_over_option_index = 0;
  }
  game_over_options[0] = NONE;
  game_over_options[1] = SINGLE;
  game_over_options[2] = DOUBLE;
  game_over_option_labels[0] = "-";
  game_over_option_labels[1] = "bzzt";
  game_over_option_labels[2] = "bzzt bzzt";

  s_game_setup.game_over = game_over_options[currently_selected_game_over_option_index];

  s_game_setup_window = window_create();

  // Get the bounds of the Window
  Layer* window_layer = window_get_root_layer(s_game_setup_window);
  GRect window_bounds = layer_get_bounds(window_layer);

  s_menu_layer = menu_layer_create(window_bounds);

  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = PBL_IF_RECT_ELSE(menu_get_header_height_callback, NULL),
    .draw_header = PBL_IF_RECT_ELSE(menu_draw_header_callback, NULL),
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback
  });

   menu_layer_set_click_config_onto_window(s_menu_layer, s_game_setup_window);

  layer_add_child(
    window_get_root_layer(s_game_setup_window),
    menu_layer_get_layer(s_menu_layer)
  );

  window_stack_push(s_game_setup_window, false);
}

void game_setup_window_destroy() {
  window_stack_remove(s_game_setup_window, false);

  menu_layer_destroy(s_menu_layer);

  window_destroy(s_game_setup_window);
}
