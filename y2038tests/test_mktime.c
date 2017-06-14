#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "tests.h"

static int test_mktime_call(
  struct tm *tm,
  time_t expected_result)
{
  time_t result = mktime(tm);
  if (result == -1) return 1;
  if (result != expected_result) return 1;
  return 0;
}

void test_mktime(void)
{
  int result;
  struct tm tm0 = { 0, 0, 0, 1, 0, 70, 4, 0, 0 };
  time_t t = 0;

  test_begin("Check that mktime(1970-01-01 00:00:00) returns 0");
  result = test_mktime_call(&tm0, t);
  if (result) test_failure(); else test_success();

  struct tm tmY2038 = { 7, 14, 3, 19, 0, 138, 2, 18, 0 };
  t = 0x7fffffff;

  test_begin("Check that mktime(2038-01-19 03:14:07) returns 0x7FFFFFFF");
  result = test_mktime_call(&tm0, t);
  result = test_mktime_call(&tmY2038, t);
  if (result) test_failure(); else test_success();

  struct tm tmY2038p = { 8, 14, 3, 19, 0, 138, 2, 18, 0 };
  t++;

  test_begin("Check that mktime(2038-01-19 03:14:08) returns 0x80000000");
  result = test_mktime_call(&tmY2038p, t);
  if (result) test_failure(); else test_success();
}
