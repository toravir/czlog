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

const char *readNBytes(FILE *fp, int n)
{
  char *buf = calloc(n, 1);
  if (!buf) {
      return NULL;
  }
  fread(buf, n, 1, fp);
  return buf;
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


START_TEST(test_logger_int) {
  logHandle *m;
  char tmpFile[] = "/tmp/libczTest_XXXXXX";
  m = newlogHandleFd(mkstemp(tmpFile), LOG_DEBUG, 0);
  ck_assert_ptr_ne(m, NULL);
  FILE *rdback = fopen(tmpFile, "r");
  char *logLine = NULL;
  size_t lineLen = 0;
  char *expectedBuf = NULL;
  setLogAutoTs(m, FALSE);

  doLog(m, LOG_WARN, L_INT("price", 1000), L_PRINT);
  expectedBuf="{\"price\":1000,\"level\":\"warn\"}";
  getline(&logLine, &lineLen, rdback);
  ck_assert_str_eq(logLine, expectedBuf);
  free((void*)logLine);

  doLog(m, LOG_WARN, L_INT("price", 0), L_PRINT);
  expectedBuf="{\"price\":0,\"level\":\"warn\"}";
  getline(&logLine, &lineLen, rdback);
  ck_assert_str_eq(logLine, expectedBuf);
  free((void*)logLine);

  doLog(m, LOG_WARN, L_INT("price", -1000), L_PRINT);
  expectedBuf="{\"price\":-1000,\"level\":\"warn\"}";
  getline(&logLine, &lineLen, rdback);
  ck_assert_str_eq(logLine, expectedBuf);
  free((void*)logLine);

  doLog(m, LOG_WARN, L_INT("price", 2147483647), L_PRINT);
  expectedBuf="{\"price\":2147483647,\"level\":\"warn\"}";
  getline(&logLine, &lineLen, rdback);
  ck_assert_str_eq(logLine, expectedBuf);
  free((void*)logLine);

  doLog(m, LOG_WARN, L_INT("price", -2147483646), L_PRINT);
  expectedBuf="{\"price\":-2147483646,\"level\":\"warn\"}";
  getline(&logLine, &lineLen, rdback);
  ck_assert_str_eq(logLine, expectedBuf);
  free((void*)logLine);

  unlink(tmpFile);
} END_TEST


START_TEST(test_logger_str) {
  logHandle *m;
  char tmpFile[] = "/tmp/libczTest_XXXXXX";
  m = newlogHandleFd(mkstemp(tmpFile), LOG_DEBUG, 0);
  ck_assert_ptr_ne(m, NULL);
  FILE *rdback = fopen(tmpFile, "r");
  char *logLine = NULL;
  size_t lineLen = 0;
  char *expectedBuf = NULL;
  setLogAutoTs(m, FALSE);

  doLog(m, LOG_WARN, L_STR("StrVal", "F"), L_PRINT);
  expectedBuf="{\"StrVal\":\"F\",\"level\":\"warn\"}";
  getline(&logLine, &lineLen, rdback);
  ck_assert_str_eq(logLine, expectedBuf);
  free((void*)logLine);

  doLog(m, LOG_WARN, L_STR("StrVal", ""), L_PRINT);
  expectedBuf="{\"StrVal\":\"\",\"level\":\"warn\"}";
  getline(&logLine, &lineLen, rdback);
  ck_assert_str_eq(logLine, expectedBuf);
  free((void*)logLine);

  doLog(m, LOG_WARN, L_STR("StrVal", "128Character string in here0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"), L_PRINT);
  expectedBuf="{\"StrVal\":\"128Character string in here0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\",\"level\":\"warn\"}";
  getline(&logLine, &lineLen, rdback);
  ck_assert_str_eq(logLine, expectedBuf);
  free((void*)logLine);

  unlink(tmpFile);
} END_TEST

START_TEST(test_logger_multiple_call) {
  logHandle *m;
  char tmpFile[] = "/tmp/libczTest_XXXXXX";
  m = newlogHandleFd(mkstemp(tmpFile), LOG_DEBUG, 0);
  ck_assert_ptr_ne(m, NULL);
  FILE *rdback = fopen(tmpFile, "r");
  char *logLine = NULL;
  size_t lineLen = 0;
  char *expectedBuf = NULL;
  setLogAutoTs(m, FALSE);

  doLog(m, LOG_WARN, L_STR("StrVal", "F"), L_MORE);
  doLog(m, LOG_WARN, L_INT("IntVal", 100), L_PRINT);
  expectedBuf="{\"StrVal\":\"F\",\"IntVal\":100,\"level\":\"warn\"}";
  getline(&logLine, &lineLen, rdback);
  ck_assert_str_eq(logLine, expectedBuf);
  free((void*)logLine);

  doLog(m, LOG_WARN, L_STR("StrVal", "F"), L_MORE);
  doLog(m, LOG_WARN, L_INT("IntVal", 100), L_MORE);
  doLog(m, LOG_WARN, L_PRINT);
  expectedBuf="{\"StrVal\":\"F\",\"IntVal\":100,\"level\":\"warn\"}";
  getline(&logLine, &lineLen, rdback);
  ck_assert_str_eq(logLine, expectedBuf);
  free((void*)logLine);

  doLog(m, LOG_WARN, L_MORE);
  doLog(m, LOG_WARN, L_STR("StrVal", "F"), L_MORE);
  doLog(m, LOG_WARN, L_INT("IntVal", 100), L_MORE);
  doLog(m, LOG_WARN, L_PRINT);
  expectedBuf="{\"StrVal\":\"F\",\"IntVal\":100,\"level\":\"warn\"}";
  getline(&logLine, &lineLen, rdback);
  ck_assert_str_eq(logLine, expectedBuf);
  free((void*)logLine);

  unlink(tmpFile);
} END_TEST

START_TEST(test_logger_levels) {
  logHandle *m;
  char tmpFile[] = "/tmp/libczTest_XXXXXX";
  m = newlogHandleFd(mkstemp(tmpFile), LOG_ERROR, 0);
  ck_assert_ptr_ne(m, NULL);
  FILE *rdback = fopen(tmpFile, "r");
  char *logLine = NULL;
  size_t lineLen = 0;
  char *expectedBuf = NULL;
  setLogAutoTs(m, FALSE);

  doLog(m, LOG_WARN, L_INT("price", 1000), L_STR("Desc", "Without TS"), L_PRINT);
  doLog(m, LOG_ERROR, L_INT("price", 1000), L_STR("Desc", "Error level string"), L_PRINT);
  expectedBuf="{\"price\":1000,\"Desc\":\"Error level string\",\"level\":\"error\"}";
  getline(&logLine, &lineLen, rdback);
  ck_assert_str_eq(logLine, expectedBuf);
  free((void*)logLine);

  doLog(m, LOG_FATAL, L_INT("price", 1000), L_STR("Desc", "Fatal level string"), L_PRINT);
  expectedBuf="{\"price\":1000,\"Desc\":\"Fatal level string\",\"level\":\"fatal\"}";
  getline(&logLine, &lineLen, rdback);
  ck_assert_str_eq(logLine, expectedBuf);
  free((void*)logLine);

  doLog(m, LOG_PANIC, L_INT("price", 1000), L_STR("Desc", "Panic level string"), L_PRINT);
  expectedBuf="{\"price\":1000,\"Desc\":\"Panic level string\",\"level\":\"panic\"}";
  getline(&logLine, &lineLen, rdback);
  ck_assert_str_eq(logLine, expectedBuf);
  free((void*)logLine);

  setLogLevel(m, LOG_FATAL);
  doLog(m, LOG_INFO,  L_INT("price", 1000), L_STR("Desc", "Info level string"),  L_PRINT);
  doLog(m, LOG_DEBUG, L_INT("price", 1000), L_STR("Desc", "Debug level string"), L_PRINT);
  doLog(m, LOG_ERROR, L_INT("price", 1000), L_STR("Desc", "Error level string"), L_PRINT);
  doLog(m, LOG_FATAL, L_INT("price", 1000), L_STR("Desc", "Fatal level string"), L_PRINT);
  expectedBuf="{\"price\":1000,\"Desc\":\"Fatal level string\",\"level\":\"fatal\"}";
  getline(&logLine, &lineLen, rdback);
  ck_assert_str_eq(logLine, expectedBuf);
  free((void*)logLine);

  unlink(tmpFile);
} END_TEST

START_TEST(test_logger_context) {
  logHandle *m;
  char tmpFile[] = "/tmp/libczTest_XXXXXX";
  m = newlogHandleFd(mkstemp(tmpFile), LOG_INFO, 0);
  ck_assert_ptr_ne(m, NULL);
  FILE *rdback = fopen(tmpFile, "r");
  char *logLine = NULL;
  size_t lineLen = 0;
  char *expectedBuf = NULL;
  setLogAutoTs(m, FALSE);

  doLog(m, LOG_WARN, L_INT("price", 1000), L_STR("Desc", "Without TS"), L_MORE);
  saveToCtx(m);

  logHandle *mctx = cloneHdl(m);
  clearCtx(m);

  doLog(mctx, LOG_ERROR, L_STR("Ctx", "in m1 context"), L_PRINT);
  expectedBuf="{\"price\":1000,\"Desc\":\"Without TS\",\"Ctx\":\"in m1 context\",\"level\":\"error\"}";
  getline(&logLine, &lineLen, rdback);
  ck_assert_str_eq(logLine, expectedBuf);
  free((void*)logLine);

  doLog(m, LOG_FATAL, L_INT("price", 1000), L_STR("Desc", "Fatal level string"), L_PRINT);
  expectedBuf="{\"price\":1000,\"Desc\":\"Fatal level string\",\"level\":\"fatal\"}";
  getline(&logLine, &lineLen, rdback);
  ck_assert_str_eq(logLine, expectedBuf);
  free((void*)logLine);

  doLog(mctx, LOG_PANIC, L_INT("Sub Price", 1000), L_PRINT);
  expectedBuf="{\"price\":1000,\"Desc\":\"Without TS\",\"Sub Price\":1000,\"level\":\"panic\"}";
  getline(&logLine, &lineLen, rdback);
  ck_assert_str_eq(logLine, expectedBuf);
  free((void*)logLine);

  unlink(tmpFile);
} END_TEST

START_TEST(test_bin_logger_create) {
  logHandle *m;
  char tmpFile[] = "/tmp/libczTest_XXXXXX";
  m = newlogHandleFd(mkstemp(tmpFile), LOG_DEBUG, 1);
  ck_assert_ptr_ne(m, NULL);
  setLogAutoTs(m, FALSE);
  doLog(m, LOG_WARN, L_INT("price", 1000), L_STR("Desc", "Without TS"), L_PRINT);
  FILE *rdback = fopen(tmpFile, "rb");
  const char expected[] = {
         0xbf, 0x65, 0x70, 0x72, 0x69, 0x63, 0x65, 0x19, 0x03, 0xe8, 0x64, 0x44, 0x65, 0x73, 0x63, 0x6a,
         0x57, 0x69, 0x74, 0x68, 0x6f, 0x75, 0x74, 0x20, 0x54, 0x53, 0x65, 0x6c, 0x65, 0x76, 0x65, 0x6c,
         0x64, 0x77, 0x61, 0x72, 0x6e, 0xff,
  };
  const char *bytes = readNBytes(rdback, sizeof(expected));
  for (int i = 0; i < sizeof(expected); i++)
      ck_assert_msg(bytes[i] == expected[i],
              "At index: %d, expected: 0x%02X, got 0x%02X", i, expected[i], bytes[i]);

  free((void*)bytes);
  unlink(tmpFile);
} END_TEST



Suite *logger_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("logTestSuite");
  tc_core = tcase_create("logger");

  tcase_add_test(tc_core, test_logger_create);
  tcase_add_test(tc_core, test_logger_int);
  tcase_add_test(tc_core, test_logger_str);
  tcase_add_test(tc_core, test_logger_multiple_call);
  tcase_add_test(tc_core, test_logger_levels);
  tcase_add_test(tc_core, test_logger_context);

  tcase_add_test(tc_core, test_bin_logger_create);
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
