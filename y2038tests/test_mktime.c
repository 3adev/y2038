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
  struct tm tm0 = {.tm_year = 70, .tm_mon = 0, .tm_mday = 1, .tm_hour = 0, .tm_min = 0, .tm_sec = 0, .tm_wday = 4, .tm_yday = 0 };
  time_t t = 0;
  test_mktime_call(&tm0, t);

  test_begin("Check that mktime(2038-01-19 03:14:07) returns 0x7FFFFFFF");
  struct tm tmY2038 = {.tm_year = 138, .tm_mon = 0, .tm_mday = 19, .tm_hour = 3, .tm_min = 14, .tm_sec = 7, .tm_wday = 2, .tm_yday = 18 };
  t = 0x7fffffff;
  test_mktime_call(&tmY2038, t);

  test_begin("Check that mktime(2038-01-19 03:14:08) returns 0x80000000");
  struct tm tmY2038p = {.tm_year = 138, .tm_mon = 0, .tm_mday = 19, .tm_hour = 3, .tm_min = 14, .tm_sec = 8, .tm_wday = 2, .tm_yday = 18 };
  t++;
  test_mktime_call(&tmY2038p, t);
}
