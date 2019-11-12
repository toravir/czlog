#include <check.h>
#include "../include/logger.h"
#include <stdlib.h>
#include <unistd.h>

const char *readLineFromFile(const char *fName)
{
  FILE *rdback = fopen(fName, "r");
  ck_assert_ptr_ne(rdback, NULL);
  char *logLine = NULL;
  size_t lineLen = 0;
  getline(&logLine, &lineLen, rdback);
  fclose(rdback);
  return logLine;
}

START_TEST(test_logger_create) {
  logHandle *m;
  char tmpFile[] = "/tmp/libczTest_XXXXXX";
  m = newlogHandleFd(mkstemp(tmpFile), LOG_DEBUG, 0);
  ck_assert_ptr_ne(m, NULL);
  setLogAutoTs(m, FALSE);
  doLog(m, LOG_WARN, L_INT("price", 1000), L_STR("Desc", "Without TS"), L_PRINT);
  const char *expectedBuf="{\"price\":1000,\"Desc\":\"Without TS\",\"level\":\"warn\"}";
  const char *logLine = readLineFromFile(tmpFile);
  ck_assert_str_eq(logLine, expectedBuf);
  free((void*)logLine);
  unlink(tmpFile);
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
