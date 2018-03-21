#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "tests.h"

static void test_mktime_call(
  struct tm *tm,
  time_t expected_result)
{
  time_t result = mktime(tm);
  if (result == -1)
    test_failure(1, "mktime returned %lld", (long long) result);
  else if (result != expected_result)
    test_failure(0, "mktime returned %lld instead of %lld", (long long) result, (long long) expected_result);
  else
    test_success();
}

void test_mktime(void)
{
  test_begin("Check that mktime(1970-01-01 00:00:00) returns 0");
  struct tm tm0 = { 0, 0, 0, 1, 0, 70, 4, 0, 0 };
  time_t t = 0;
  test_mktime_call(&tm0, t);

  test_begin("Check that mktime(2038-01-19 03:14:07) returns 0x7FFFFFFF");
  struct tm tmY2038 = { 7, 14, 3, 19, 0, 138, 2, 18, 0 };
  t = 0x7fffffff;
  test_mktime_call(&tmY2038, t);

  test_begin("Check that mktime(2038-01-19 03:14:08) returns 0x80000000");
  struct tm tmY2038p = { 8, 14, 3, 19, 0, 138, 2, 18, 0 };
  t++;
  test_mktime_call(&tmY2038p, t);
}
