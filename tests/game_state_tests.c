#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <check.h>

#include "src/table_tennis.h"

START_TEST (two_player_short_game_starting_state)
{
  TableTennis* table_tennis = table_tennis_create(SHORT_GAME, TEAM_1);

  // check initial game state
  ck_assert_int_eq(table_tennis->total_score, 0);
  ck_assert(table_tennis->winner == NO_TEAM);
  ck_assert(table_tennis->serve_just_changed == false);
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

START_TEST (two_player_short_game_simple)
{
  TableTennis* table_tennis = table_tennis_create(SHORT_GAME, TEAM_1);

  // play first rally and score player 1
  table_tennis_increment_score(table_tennis, TEAM_1); // 1-0

  // check state (player 1 score should have incremented)
  ck_assert_int_eq(table_tennis->total_score, 1);
  ck_assert(table_tennis->serve_just_changed == false);
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
  ck_assert(table_tennis->serve_just_changed == true);
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
  ck_assert(table_tennis->overtime == false);
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

START_TEST (two_player_long_game_simple)
{
  TableTennis* table_tennis = table_tennis_create(LONG_GAME, TEAM_1);

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

  // check that the SHORT_GAME (11) endgame logic has not happened
  ck_assert(table_tennis->winner == NO_TEAM);

  table_tennis_increment_score(table_tennis, TEAM_1); // 12-1
  table_tennis_increment_score(table_tennis, TEAM_1); // 13-1
  table_tennis_increment_score(table_tennis, TEAM_1); // 14-1
  table_tennis_increment_score(table_tennis, TEAM_1); // 15-1
  table_tennis_increment_score(table_tennis, TEAM_1); // 16-1
  table_tennis_increment_score(table_tennis, TEAM_1); // 17-1
  table_tennis_increment_score(table_tennis, TEAM_1); // 18-1
  table_tennis_increment_score(table_tennis, TEAM_1); // 19-1
  table_tennis_increment_score(table_tennis, TEAM_1); // 20-1
  table_tennis_increment_score(table_tennis, TEAM_1); // 21-1

  // check state (player 2 score should have incremented, and should they should now be serving)
  ck_assert_int_eq(table_tennis->total_score, 22);
  ck_assert(table_tennis->winner == TEAM_1);
  ck_assert(table_tennis->overtime == false);
  // player 1
  ck_assert_int_eq(table_tennis->teams[TEAM_1].score, 21);
  ck_assert_str_eq(table_tennis->teams[TEAM_1].score_formatted, "21");
  ck_assert(table_tennis->teams[TEAM_1].serving == false);
  ck_assert(table_tennis->teams[TEAM_1].serving_first == true);
  // player 2
  ck_assert_int_eq(table_tennis->teams[TEAM_2].score, 1);
  ck_assert_str_eq(table_tennis->teams[TEAM_2].score_formatted, "1");
  ck_assert(table_tennis->teams[TEAM_2].serving == true);
  ck_assert(table_tennis->teams[TEAM_2].serving_first == false);

  table_tennis_destroy(table_tennis);
}
END_TEST

START_TEST (two_player_short_game_endgame)
{
  TableTennis* table_tennis = table_tennis_create(SHORT_GAME, TEAM_1);

  // play until 10:0 and verify overtime is not activated
  for (int i = 0; i < 10; i++) {
    table_tennis_increment_score(table_tennis, TEAM_1);
    ck_assert(table_tennis->winner == NO_TEAM);
  }

  table_tennis_increment_score(table_tennis, TEAM_1); // 11:0

  // verify we are in overtime
  ck_assert(table_tennis->winner == TEAM_1);

  table_tennis_destroy(table_tennis);
}
END_TEST

START_TEST (two_player_long_game_endgame)
{
  TableTennis* table_tennis = table_tennis_create(LONG_GAME, TEAM_1);

  // play until 20:0 and verify overtime is not activated
  for (int i = 0; i < 20; i++) {
    table_tennis_increment_score(table_tennis, TEAM_1);
    ck_assert(table_tennis->winner == NO_TEAM);
  }

  table_tennis_increment_score(table_tennis, TEAM_1); // 21:0

  // verify we are in overtime
  ck_assert(table_tennis->winner == TEAM_1);

  table_tennis_destroy(table_tennis);
}
END_TEST

START_TEST (two_player_short_game_overtime)
{
  TableTennis* table_tennis = table_tennis_create(SHORT_GAME, TEAM_1);

  // play until 9:9 and verify overtime is not activated
  for (int i = 0; i < 9; i++) {
    table_tennis_increment_score(table_tennis, TEAM_1);
    table_tennis_increment_score(table_tennis, TEAM_2);
    ck_assert(table_tennis->overtime == false);
  }

  table_tennis_increment_score(table_tennis, TEAM_1); // 10:9
  table_tennis_increment_score(table_tennis, TEAM_2); // 10:10

  // verify we are in overtime
  ck_assert(table_tennis->overtime == true);

  //ck_assert(table_tennis->teams[TEAM_1].serving == true);
  //ck_assert(table_tennis->teams[TEAM_2].serving == false);

  //table_tennis_increment_score(table_tennis, TEAM_1); // 11-10

  //ck_assert(table_tennis->teams[TEAM_1].serving == false);
  //ck_assert(table_tennis->teams[TEAM_2].serving == true);

  //table_tennis_increment_score(table_tennis, TEAM_1); // 12-10

  //ck_assert(table_tennis->teams[TEAM_1].serving == true);
  //ck_assert(table_tennis->teams[TEAM_2].serving == false);

  //ck_assert(table_tennis->winner == TEAM_1);

  table_tennis_destroy(table_tennis);
}
END_TEST

START_TEST (two_player_long_game_overtime)
{
  TableTennis* table_tennis = table_tennis_create(LONG_GAME, TEAM_1);

  // play until 19:19 and verify overtime is not activated
  for (int i = 0; i < 19; i++) {
    table_tennis_increment_score(table_tennis, TEAM_1);
    table_tennis_increment_score(table_tennis, TEAM_2);
    ck_assert(table_tennis->overtime == false);
  }

  table_tennis_increment_score(table_tennis, TEAM_1); // 20:19
  table_tennis_increment_score(table_tennis, TEAM_2); // 20:20

  // verify we are in overtime
  ck_assert(table_tennis->overtime == true);

  //ck_assert(table_tennis->teams[TEAM_1].serving == true);
  //ck_assert(table_tennis->teams[TEAM_2].serving == false);

  //table_tennis_increment_score(table_tennis, TEAM_1); // 21-20

  //ck_assert(table_tennis->teams[TEAM_1].serving == false);
  //ck_assert(table_tennis->teams[TEAM_2].serving == true);

  //table_tennis_increment_score(table_tennis, TEAM_1); // 22-20

  //ck_assert(table_tennis->teams[TEAM_1].serving == true);
  //ck_assert(table_tennis->teams[TEAM_2].serving == false);

  //ck_assert(table_tennis->winner == TEAM_1);

  table_tennis_destroy(table_tennis);
}
END_TEST

Suite* table_tennis_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("Game State");

  /* Core test case */
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, two_player_short_game_starting_state);
  tcase_add_test(tc_core, two_player_short_game_simple);
  tcase_add_test(tc_core, two_player_long_game_simple);
  tcase_add_test(tc_core, two_player_short_game_endgame);
  tcase_add_test(tc_core, two_player_long_game_endgame);
  tcase_add_test(tc_core, two_player_short_game_overtime);
  tcase_add_test(tc_core, two_player_long_game_overtime);
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
