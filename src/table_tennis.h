#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

//
// Table Tennis
// Handles the game state and rules of a game of table tennis.
//
// DONE:
// - singles
// - games to 11
// - games to 21
// - win by 2
//
// TODO:
// - doubles
//

typedef enum {
  SHORT_GAME = 11,
  LONG_GAME = 21
} GameLength;

typedef enum {
  TEAM_1 = 0,
  TEAM_2 = 1,
  NO_TEAM = 99
} TeamNumber;

// typedef void (*team_serving_changed_callback)(TeamNumber team_number);
// typedef void (*team_score_changed_callback)(TeamNumber team_number, int new_score, char new_score_formatted[4]);
// typedef void (*team_win_callback)(TeamNumber team_number);
//
// typedef struct {
//   team_serving_changed_callback serving_team_changed;
//   team_score_changed_callback team_score_changed;
//   team_win_callback team_win;
// } GameStateChangeCallbacks;

typedef struct {
  int score;
  char score_formatted[4];
  bool serving;
  bool serving_first;
} Team;

typedef struct {
  Team teams[2];
  TeamNumber winner;
  int total_score;
  bool overtime;
  GameLength game_length;
} TableTennis;

TableTennis* table_tennis_create(GameLength game_length, TeamNumber starting_team /*, GameStateChangeCallbacks table_tennis_change_callbacks */);
void table_tennis_destroy(TableTennis* table_tennis);
void table_tennis_increment_score(TableTennis* table_tennis, TeamNumber team);
