#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "tests.h"

static void test_gmtime_r_call(
  const time_t *timep,
  const struct tm *expected_result)
{
  struct tm r, *result;
  result = gmtime_r(timep, &r);
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
  if (failed) test_success;
}

void test_gmtime_r(void)
{
  test_begin("Check that gmtime_r(0) yields 1970-01-01 00:00:00");
  struct tm tm0 = { 0, 0, 0, 1, 0, 70, 4, 0, 0 };
  time_t t = 0;
  test_gmtime_r_call(&t, &tm0);

  test_begin("Check that gmtime_r(0x7FFFFFFF) yields 2038-01-19 03:14:07");
  struct tm tmY2038 = { 7, 14, 3, 19, 0, 138, 2, 18, 0 };
  t = 0x7fffffff;
  test_gmtime_r_call(&t, &tmY2038);

  test_begin("Check that gmtime_r(0x7FFFFFFF+1) yields 2038-01-19 03:14:08");
  struct tm tmY2038p = { 8, 14, 3, 19, 0, 138, 2, 18, 0 };
  t++;
  test_gmtime_r_call(&t, &tmY2038p);

  test_begin("Check that gmtime_r(0x80000000) yields 2038-01-19 03:14:08");
  t = 0x80000000;
  test_gmtime_r_call(&t, &tmY2038p);

  test_begin("Check that gmtime_r(0x80000000-1) yields 2038-01-19 03:14:07");
  t--;
  test_gmtime_r_call(&t, &tmY2038);
}
