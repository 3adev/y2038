#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "tests.h"

static void test_gmtime_call(
  const time_t *timep,
  const struct tm *expected_result)
{
  const struct tm *result = gmtime(timep);
  if (!result)
  {
    test_failure(1, "gmtime returned NULL");
    return;
  }
  int failed = 0;
  if (result->tm_sec != expected_result->tm_sec)
  {
    test_failure(0, "gmtime returned tm_sec = %d instead of %d", result->tm_sec, expected_result->tm_sec);
    failed = 1;
  }
  if (result->tm_min != expected_result->tm_min)
  {
    test_failure(0, "gmtime returned tm_min = %d instead of %d", result->tm_min, expected_result->tm_min);
    failed = 1;
  }
  if (result->tm_hour != expected_result->tm_hour)
  {
    test_failure(0, "gmtime returned tm_hour = %d instead of %d", result->tm_hour, expected_result->tm_hour);
    failed = 1;
  }
  if (result->tm_mday != expected_result->tm_mday)
  {
    test_failure(0, "gmtime returned tm_mday = %d instead of %d", result->tm_mday, expected_result->tm_mday);
    failed = 1;
  }
  if (result->tm_mon != expected_result->tm_mon)
  {
    test_failure(0, "gmtime returned tm_mon = %d instead of %d", result->tm_mon, expected_result->tm_mon);
    failed = 1;
  }
  if (result->tm_year != expected_result->tm_year)
  {
    test_failure(0, "gmtime returned tm_year = %d instead of %d", result->tm_year, expected_result->tm_year);
    failed = 1;
  }
  if (result->tm_wday != expected_result->tm_wday)
  {
    test_failure(0, "gmtime returned tm_wday = %d instead of %d", result->tm_wday, expected_result->tm_wday);
    failed = 1;
  }
  if (result->tm_yday != expected_result->tm_yday)
  {
    test_failure(0, "gmtime returned tm_yday = %d instead of %d", result->tm_yday, expected_result->tm_yday);
    failed = 1;
  }
  if (result->tm_isdst != expected_result->tm_isdst)
  {
    test_failure(0, "gmtime returned tm_isdst = %d instead of %d", result->tm_isdst, expected_result->tm_isdst);
    failed = 1;
  }
  if (!failed) test_success();
}

void test_gmtime(void)
{
  test_begin("Check that gmtime(0) yields 1970-01-01 00:00:00");
  struct tm tm0 = {.tm_year = 70, .tm_mon = 0, .tm_mday = 1, .tm_hour = 0, .tm_min = 0, .tm_sec = 0, .tm_wday = 4, .tm_yday = 0 };
  time_t t = 0;
  test_gmtime_call(&t, &tm0);

  test_begin("Check that gmtime(0x7FFFFFFF) yields 2038-01-19 03:14:07");
  struct tm tmY2038 = {.tm_year = 138, .tm_mon = 0, .tm_mday = 19, .tm_hour = 3, .tm_min = 14, .tm_sec = 7, .tm_wday = 2, .tm_yday = 18 };
  t = 0x7fffffff;
  test_gmtime_call(&t, &tmY2038);

  test_begin("Check that gmtime(0x7FFFFFFF+1) yields 2038-01-19 03:14:08");
  struct tm tmY2038p = {.tm_year = 138, .tm_mon = 0, .tm_mday = 19, .tm_hour = 3, .tm_min = 14, .tm_sec = 8, .tm_wday = 2, .tm_yday = 18 };
  t++;
  test_gmtime_call(&t, &tmY2038p);

  test_begin("Check that gmtime(0x80000000) yields 2038-01-19 03:14:08");
  t = 0x80000000;
  test_gmtime_call(&t, &tmY2038p);

  test_begin("Check that gmtime(0x80000000-1) yields 2038-01-19 03:14:08 - 1");
  t--;
  test_gmtime_call(&t, &tmY2038);
}
