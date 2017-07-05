#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <string.h>
#include <stdio.h>

#include "tests.h"

static int test_gettimeofday(const struct timeval *tv)
{
  struct timeval t0;
  int result = gettimeofday(&t0, NULL);
  double d = difftime(t0.tv_sec, tv->tv_sec);
  if (d < -1.0 || d > 1.0) return 1;
  return 0;
}

static int test_settimeofday(const struct timeval *tv)
{
  int result = settimeofday(tv, NULL);
  if (result) return 1;
  return 0;
}

void test_gettimeofday_settimeofday(void)
{
  struct timeval t0, t;

  test_begin("Save current time of day");
  int result = gettimeofday(&t0, NULL);
  if (result) test_failure(); else test_success();

  test_begin("Set time of day to Y2038 minus 60 seconds");
  t.tv_sec = 0x7FFFFFFF;
  t.tv_sec -= 59;
  t.tv_usec = 0;
  result = test_settimeofday(&t);
  if (result) test_failure(); else test_success();
  
  if (result == 0)
  {
    test_begin("Check time of day against Y2038-60s");
    result = test_gettimeofday(&t);
    if (result) test_failure(); else test_success();
  }

  test_begin("Set time of day to Y2038 plus 60 seconds");
  t.tv_sec = 0x7FFFFFFF;
  t.tv_sec += 61;
  t.tv_usec = 0;
  result = test_settimeofday(&t);
  if (result) test_failure(); else test_success();

  if (result == 0)
  {
    test_begin("Check time of day against Y2038+60s");
    result = test_gettimeofday(&t);
    if (result) test_failure(); else test_success();
  }

  test_begin("Restore time of day");
  result = settimeofday(&t0, NULL);
  if (result) test_failure(); else test_success();
}
