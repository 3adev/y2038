#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "tests.h"

static void test_clock_getres(struct timespec *tv, time_t sec)
{
  int result = clock_getres(CLOCK_REALTIME, tv);
  if (result)
    test_failure(1, "clock_getres returned %d", result);
  else if (tv->tv_sec != sec)
    test_failure(0, "clock_getres returned %lld instead of %lld", (long long) (tv->tv_sec), (long long) sec);
  else
    test_success();
}

static void test_clock_gettime(struct timespec *tv, time_t sec)
{
  int result = clock_gettime(CLOCK_REALTIME, tv);
  if (result)
    test_failure(1, "clock_gettime returned %d", result);
  else if (tv->tv_sec != sec)
    test_failure(0, "clock_gettime returned %lld instead of %lld", (long long) (tv->tv_sec), (long long) sec);
  else
    test_success();
}

static int test_clock_settime(struct timespec *tv)
{
  int result = clock_settime(CLOCK_REALTIME, tv);
  if (result)
    test_failure(1, "clock_settime returned %d", result);
  else
    test_success();
  return result;
}

void test_clock_gettime_settime(void)
{
  struct timespec tv0, tv;

  test_begin("Get clock real time");
  memset(&tv0, 0x55, sizeof(tv0));
  int result = clock_gettime(CLOCK_REALTIME, &tv0);
  if (result) test_failure(1, "clock_gettime returned %d", result); else test_success();

  test_begin("Get clock resolution");
  tv.tv_sec = 0;
  tv.tv_nsec = 0;
  test_clock_getres(&tv, 0);

  test_begin("Set clock real time to Y2038 minus 60 seconds");
  tv.tv_sec = 0x7FFFFFFF;
  tv.tv_sec -= 59;
  tv.tv_nsec = 0;
  result = test_clock_settime(&tv);
  
  if (result == 0)
  {
    test_begin("Get clock real time");
	test_clock_gettime(&tv, tv.tv_sec);
  }

  test_begin("Set clock real time to Y2038 plus 60 seconds");
  tv.tv_sec = 0x7FFFFFFF;
  tv.tv_sec += 61;
  tv.tv_nsec = 0;
  result = test_clock_settime(&tv);

  if (result == 0)
  {
    test_begin("Get clock real time");
	test_clock_gettime(&tv, tv.tv_sec);
  }

  test_begin("Restore clock real time");
  result = clock_settime(CLOCK_REALTIME, &tv0);
  if (result) test_failure(1, "closk_settime returned %d", result); else test_success();
}
