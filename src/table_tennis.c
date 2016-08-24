#include "table_tennis.h"

void table_tennis_update_team_serving_state(TableTennis* table_tennis, TeamNumber starting_team);
void table_tennis_update_winner(TableTennis* table_tennis);

TableTennis* table_tennis_create(GameLength game_length, TeamNumber starting_team) {
  TableTennis* table_tennis = malloc(sizeof(TableTennis));

  table_tennis->total_score = 0;
  table_tennis->winner = NO_TEAM;
  table_tennis->overtime = false;
  table_tennis->serve_just_changed = false;
  table_tennis->game_length = game_length;

  table_tennis->teams[TEAM_1].score = 0;
  table_tennis->teams[TEAM_1].serving = false;
  table_tennis->teams[TEAM_1].serving_first = false;
  snprintf(table_tennis->teams[TEAM_1].score_formatted, 4, "%d", 0);

  table_tennis->teams[TEAM_2].score = 0;
  table_tennis->teams[TEAM_2].serving = false;
  table_tennis->teams[TEAM_2].serving_first = false;
  snprintf(table_tennis->teams[TEAM_2].score_formatted, 4, "%d", 0);

  table_tennis->teams[starting_team].serving = true;
  table_tennis->teams[starting_team].serving_first = true;

  return table_tennis;
}

void table_tennis_destroy(TableTennis* table_tennis) {
  free(table_tennis);
}

void table_tennis_increment_score(TableTennis* table_tennis, TeamNumber team_number) {
  // update general game state
  table_tennis->total_score += 1;

  Team* team = &(table_tennis->teams[team_number]);

  // update team score
  team->score += 1;

  // update team score formatted
  snprintf(
    team->score_formatted,
    4,
    "%d",
    team->score
  );

  // calculate if entering overtime
  if (
    table_tennis->teams[TEAM_1].score == (table_tennis->game_length - 1) &&
    table_tennis->teams[TEAM_2].score == (table_tennis->game_length - 1)
  ) {
    table_tennis->overtime = true;
  }

  bool team_1_serving = table_tennis->teams[TEAM_1].serving;

  // calculate serving state
  table_tennis_update_team_serving_state(table_tennis, TEAM_1);
  table_tennis_update_team_serving_state(table_tennis, TEAM_2);

  // calculate serve just changed
  table_tennis->serve_just_changed = team_1_serving != table_tennis->teams[TEAM_1].serving;

  // calculate winner
  table_tennis_update_winner(table_tennis);
}

void table_tennis_update_team_serving_state(TableTennis* table_tennis, TeamNumber team_number) {
  Team* team = &table_tennis->teams[team_number];

  if (table_tennis->overtime == true) {
    if (team->serving_first == true) {
      team->serving = (table_tennis->total_score - 20) % 2 == 0;
    } else {
      team->serving = (table_tennis->total_score - 20) % 2 != 0;
    }
  } else {
    if (team->serving_first == true) {
      team->serving = (table_tennis->total_score/2) % 2 == 0;
    } else {
      team->serving = (table_tennis->total_score/2) % 2 != 0;
    }
  }
}

void table_tennis_update_winner(TableTennis* table_tennis) {
  Team* team1 = &table_tennis->teams[TEAM_1];
  Team* team2 = &table_tennis->teams[TEAM_2];

  if (team1->score >= table_tennis->game_length && (team1->score - team2->score) >= 2) {
    table_tennis->winner = TEAM_1;
  }

  if (team2->score >= table_tennis->game_length && (team2->score - team1->score) >= 2) {
    table_tennis->winner = TEAM_2;
  }
}
