#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <check.h>

#include "src/table_tennis.h"

START_TEST (two_player_starting_state)
{
  TableTennis* table_tennis = table_tennis_create(SHORT_GAME, TEAM_1);

  // check initial game state
  ck_assert_int_eq(table_tennis->total_score, 0);
  ck_assert(table_tennis->winner == NO_TEAM);
  // player 1
  ck_assert_int_eq(table_tennis->teams[TEAM_1].score, 0);
  ck_assert_str_eq(table_tennis->teams[TEAM_1].score_formatted, "0");
  ck_assert(table_tennis->teams[TEAM_1].serving == true);
  ck_assert(table_tennis->teams[TEAM_1].serving_first == true);
  // player 2
  ck_assert_int_eq(table_tennis->teams[TEAM_2].score, 0);
  ck_assert_str_eq(table_tennis->teams[TEAM_2].score_formatted, "0");
  ck_assert(table_tennis->teams[TEAM_2].serving == false);
  ck_assert(table_tennis->teams[TEAM_2].serving_first == false);

  table_tennis_destroy(table_tennis);
}
END_TEST

START_TEST (two_player_simple)
{
  TableTennis* table_tennis = table_tennis_create(SHORT_GAME, TEAM_1);

  // play first rally and score player 1
  table_tennis_increment_score(table_tennis, TEAM_1); // 1-0

  // check state (player 1 score should have incremented)
  ck_assert_int_eq(table_tennis->total_score, 1);
  // player 1
  ck_assert_int_eq(table_tennis->teams[TEAM_1].score, 1);
  ck_assert_str_eq(table_tennis->teams[TEAM_1].score_formatted, "1");
  ck_assert(table_tennis->teams[TEAM_1].serving == true);
  ck_assert(table_tennis->teams[TEAM_1].serving_first == true);
  // player 2
  ck_assert_int_eq(table_tennis->teams[TEAM_2].score, 0);
  ck_assert_str_eq(table_tennis->teams[TEAM_2].score_formatted, "0");
  ck_assert(table_tennis->teams[TEAM_2].serving == false);
  ck_assert(table_tennis->teams[TEAM_2].serving_first == false);

  // play second rally and score player 2
  table_tennis_increment_score(table_tennis, TEAM_2); // 1-1

  // check state (player 2 score should have incremented, and should they should now be serving)
  ck_assert_int_eq(table_tennis->total_score, 2);
  // player 1
  ck_assert_int_eq(table_tennis->teams[TEAM_1].score, 1);
  ck_assert_str_eq(table_tennis->teams[TEAM_1].score_formatted, "1");
  ck_assert(table_tennis->teams[TEAM_1].serving == false);
  ck_assert(table_tennis->teams[TEAM_1].serving_first == true);
  // player 2
  ck_assert_int_eq(table_tennis->teams[TEAM_2].score, 1);
  ck_assert_str_eq(table_tennis->teams[TEAM_2].score_formatted, "1");
  ck_assert(table_tennis->teams[TEAM_2].serving == true);
  ck_assert(table_tennis->teams[TEAM_2].serving_first == false);

  // play until special endgame (win by two) conditions are met
  table_tennis_increment_score(table_tennis, TEAM_1); // 2-1
  table_tennis_increment_score(table_tennis, TEAM_1); // 3-1
  table_tennis_increment_score(table_tennis, TEAM_1); // 4-1
  table_tennis_increment_score(table_tennis, TEAM_1); // 5-1
  table_tennis_increment_score(table_tennis, TEAM_1); // 6-1
  table_tennis_increment_score(table_tennis, TEAM_1); // 7-1
  table_tennis_increment_score(table_tennis, TEAM_1); // 8-1
  table_tennis_increment_score(table_tennis, TEAM_1); // 9-1
  table_tennis_increment_score(table_tennis, TEAM_1); // 10-1
  table_tennis_increment_score(table_tennis, TEAM_1); // 11-1

  // check state (player 2 score should have incremented, and should they should now be serving)
  ck_assert_int_eq(table_tennis->total_score, 12);
  ck_assert(table_tennis->winner == TEAM_1);
  // player 1
  ck_assert_int_eq(table_tennis->teams[TEAM_1].score, 11);
  ck_assert_str_eq(table_tennis->teams[TEAM_1].score_formatted, "11");
  ck_assert(table_tennis->teams[TEAM_1].serving == true);
  ck_assert(table_tennis->teams[TEAM_1].serving_first == true);
  // player 2
  ck_assert_int_eq(table_tennis->teams[TEAM_2].score, 1);
  ck_assert_str_eq(table_tennis->teams[TEAM_2].score_formatted, "1");
  ck_assert(table_tennis->teams[TEAM_2].serving == false);
  ck_assert(table_tennis->teams[TEAM_2].serving_first == false);

  table_tennis_destroy(table_tennis);
}
END_TEST

START_TEST (two_player_overtime)
{
  TableTennis* table_tennis = table_tennis_create(SHORT_GAME, TEAM_1);

  // play until special endgame (win by two) conditions are met
  table_tennis_increment_score(table_tennis, TEAM_1); // 1-0
  table_tennis_increment_score(table_tennis, TEAM_2); // 1-1
  table_tennis_increment_score(table_tennis, TEAM_1); // 2-1
  table_tennis_increment_score(table_tennis, TEAM_2); // 2-2
  table_tennis_increment_score(table_tennis, TEAM_1); // 3-2
  table_tennis_increment_score(table_tennis, TEAM_2); // 3-3
  table_tennis_increment_score(table_tennis, TEAM_1); // 4-3
  table_tennis_increment_score(table_tennis, TEAM_2); // 4-4
  table_tennis_increment_score(table_tennis, TEAM_1); // 5-4
  table_tennis_increment_score(table_tennis, TEAM_2); // 5-5
  table_tennis_increment_score(table_tennis, TEAM_1); // 6-5
  table_tennis_increment_score(table_tennis, TEAM_2); // 6-6
  table_tennis_increment_score(table_tennis, TEAM_1); // 7-6
  table_tennis_increment_score(table_tennis, TEAM_2); // 7-7
  table_tennis_increment_score(table_tennis, TEAM_1); // 8-7
  table_tennis_increment_score(table_tennis, TEAM_2); // 8-8
  table_tennis_increment_score(table_tennis, TEAM_1); // 9-8
  table_tennis_increment_score(table_tennis, TEAM_2); // 9-9
  table_tennis_increment_score(table_tennis, TEAM_1); // 10-9
  table_tennis_increment_score(table_tennis, TEAM_2); // 10-10

  // verify we are in overtime
  ck_assert(table_tennis->overtime == true);

  ck_assert(table_tennis->teams[TEAM_1].serving == true);
  ck_assert(table_tennis->teams[TEAM_2].serving == false);

  table_tennis_increment_score(table_tennis, TEAM_1); // 11-10

  ck_assert(table_tennis->teams[TEAM_1].serving == false);
  ck_assert(table_tennis->teams[TEAM_2].serving == true);

  table_tennis_increment_score(table_tennis, TEAM_1); // 12-10

  ck_assert(table_tennis->teams[TEAM_1].serving == true);
  ck_assert(table_tennis->teams[TEAM_2].serving == false);

  ck_assert(table_tennis->winner == TEAM_1);

  table_tennis_destroy(table_tennis);
}
END_TEST

Suite* table_tennis_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("Game State");

  /* Core test case */
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, two_player_starting_state);
  tcase_add_test(tc_core, two_player_simple);
  tcase_add_test(tc_core, two_player_overtime);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int number_failed;
  Suite* s;
  SRunner* sr;

  s = table_tennis_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
