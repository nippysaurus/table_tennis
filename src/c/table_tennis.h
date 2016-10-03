#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef enum {
  SHORT_GAME = 11,
  LONG_GAME = 21
} GameLength;

typedef enum {
  TEAM_1 = 0,
  TEAM_2 = 1,
  NO_TEAM = 99
} TeamNumber;

typedef struct {
  uint8_t score;
  char score_formatted[4];
  bool serving;
  bool serving_first;
} Team;

#define HISTORY_LENGTH 100

typedef struct {
  Team teams[2];
  TeamNumber winner;
  uint8_t total_serve_count;
  TeamNumber history[HISTORY_LENGTH];
  uint8_t total_score;
  bool overtime;
  bool serve_just_changed;
  GameLength game_length;
} TableTennis;

TableTennis* table_tennis_create(GameLength game_length, TeamNumber starting_team);
void table_tennis_destroy(TableTennis* table_tennis);
void table_tennis_increment_score(TableTennis* table_tennis, TeamNumber team);
void table_tennis_undo_score(TableTennis* table_tennis);
bool table_tennis_can_undo(TableTennis* table_tennis);
