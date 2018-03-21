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
  int result;

  test_begin("Check that gmtime_r(0) yields 1970-01-01 00:00:00");
  struct tm tm0 = { 0, 0, 0, 1, 0, 70, 4, 0, 0 };
  time_t t = 0;
  test_timegm_call(&tm0, t);

  test_begin("Check that gmtime_r(0x7FFFFFFF) yields 2038-01-19 03:14:07");
  struct tm tmY2038 = { 7, 14, 3, 19, 0, 138, 2, 18, 0 };
  t = 0x7fffffff;
  test_timegm_call(&tmY2038, t);

  test_begin("Check that gmtime_r(0x7FFFFFFF+1) yields 2038-01-19 03:14:08");
  struct tm tmY2038p = { 8, 14, 3, 19, 0, 138, 2, 18, 0 };
  t++;
  test_timegm_call(&tmY2038p, t);

  test_begin("Check that gmtime_r(0x80000000) yields 2038-01-19 03:14:08");
  t = 0x80000000;
  test_timegm_call(&tmY2038p, t);

  test_begin("Check that gmtime_r(0x80000000-1) yields 2038-01-19 03:14:07");
  t--;
  test_timegm_call(&tmY2038, t);
}
