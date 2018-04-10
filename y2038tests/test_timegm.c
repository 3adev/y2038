#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "tests.h"

static void test_timegm_call(
  struct tm *tm,
  time_t expected_result)
{
  time_t result = timegm(tm);
  if (result == -1)
    test_failure(1, "timegm returned %lld", (long long) result);
  else if (result != expected_result)
    test_failure(0, "timegm returned %lld instead of %lld", (long long) result, (long long) expected_result);
  else
    test_success();
}

void test_timegm(void)
{
  test_begin("Check that gmtime_r(0) yields 1970-01-01 00:00:00");
  struct tm tm0 = {.tm_year = 70, .tm_mon = 0, .tm_mday = 1, .tm_hour = 0, .tm_min = 0, .tm_sec = 0, .tm_wday = 4, .tm_yday = 0 };
  time_t t = 0;
  test_timegm_call(&tm0, t);

  test_begin("Check that gmtime_r(0x7FFFFFFF) yields 2038-01-19 03:14:07");
  struct tm tmY2038 = {.tm_year = 138, .tm_mon = 0, .tm_mday = 19, .tm_hour = 3, .tm_min = 14, .tm_sec = 7, .tm_wday = 2, .tm_yday = 18 };
  t = 0x7fffffff;
  test_timegm_call(&tmY2038, t);

  test_begin("Check that gmtime_r(0x7FFFFFFF+1) yields 2038-01-19 03:14:08");
  struct tm tmY2038p = {.tm_year = 138, .tm_mon = 0, .tm_mday = 19, .tm_hour = 3, .tm_min = 14, .tm_sec = 8, .tm_wday = 2, .tm_yday = 18 };
  t++;
  test_timegm_call(&tmY2038p, t);

  test_begin("Check that gmtime_r(0x80000000) yields 2038-01-19 03:14:08");
  t = 0x80000000;
  test_timegm_call(&tmY2038p, t);

  test_begin("Check that gmtime_r(0x80000000-1) yields 2038-01-19 03:14:07");
  t--;
  test_timegm_call(&tmY2038, t);
}
