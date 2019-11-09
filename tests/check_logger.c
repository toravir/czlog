#include <check.h>
#include "../include/logger.h"
#include <stdlib.h>


START_TEST(test_logger_create) {
  logHandle *m;
  char tmpFile[] = "/tmp/libczTest_XXXXXX";
  m = newlogHandleFd(mkstemp(tmpFile), LOG_DEBUG, 0);
  ck_assert_ptr_ne(m, NULL);
  doLog(m, LOG_WARN, L_INT("price", 1000), L_STR("Desc", "Without TS"), L_PRINT);

  //unlink(tmpFile);
} END_TEST

Suite *logger_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("logTestSuite");
  tc_core = tcase_create("logger");

  tcase_add_test(tc_core, test_logger_create);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int no_failed = 0;
  Suite *s;
  SRunner *runner;

  s = logger_suite();
  runner = srunner_create(s);

  srunner_run_all(runner, CK_NORMAL);
  no_failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
