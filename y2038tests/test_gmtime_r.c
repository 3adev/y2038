#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "tests.h"

static int test_gmtime_r_call(
  const time_t *timep,
  const struct tm *expected_result)
{
  struct tm result;
  if (gmtime_r(timep, &result) == NULL) return 1;
  if (result.tm_sec != expected_result->tm_sec ||
      result.tm_min != expected_result->tm_min ||
      result.tm_hour != expected_result->tm_hour ||
      result.tm_mday != expected_result->tm_mday ||
      result.tm_mon != expected_result->tm_mon ||
      result.tm_year != expected_result->tm_year ||
      result.tm_wday != expected_result->tm_wday ||
      result.tm_yday != expected_result->tm_yday ||
      result.tm_isdst != expected_result->tm_isdst) return 1;
  return 0;
}

void test_gmtime_r(void)
{
  int result;

  test_begin("Check that gmtime_r(0) yields 1970-01-01 00:00:00");
  struct tm tm0 = { 0, 0, 0, 1, 0, 70, 4, 0, 0 };
  time_t t = 0;
  result = test_gmtime_r_call(&t, &tm0);
  if (result) test_failure(); else test_success();

  test_begin("Check that gmtime_r(0x7FFFFFFF) yields 2038-01-19 03:14:07");
  struct tm tmY2038 = { 7, 14, 3, 19, 0, 138, 2, 18, 0 };
  t = 0x7fffffff;
  result = test_gmtime_r_call(&t, &tmY2038);
  if (result) test_failure(); else test_success();

  test_begin("Check that gmtime_r(0x7FFFFFFF+1) yields 2038-01-19 03:14:08");
  struct tm tmY2038p = { 8, 14, 3, 19, 0, 138, 2, 18, 0 };
  t++;
  result = test_gmtime_r_call(&t, &tmY2038p);
  if (result) test_failure(); else test_success();

  test_begin("Check that gmtime_r(0x80000000) yields 2038-01-19 03:14:08");
  t = 0x80000000;
  result = test_gmtime_r_call(&t, &tmY2038p);
  if (result) test_failure(); else test_success();

  test_begin("Check that gmtime_r(0x80000000-1) yields 2038-01-19 03:14:07");
  t--;
  result = test_gmtime_r_call(&t, &tmY2038);
  if (result) test_failure(); else test_success();
}
