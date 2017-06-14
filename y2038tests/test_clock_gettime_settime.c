#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "tests.h"

static int test_clock_getres(struct timespec *tv, time_t sec)
{
  int result = clock_getres(CLOCK_REALTIME, tv);
  if (result) return 1;
  if (tv->tv_sec != sec) return 1;
  return 0;
}

static int test_clock_gettime(struct timespec *tv, time_t sec)
{
  int result = clock_gettime(CLOCK_REALTIME, tv);
  if (result) return 1;
  if (tv->tv_sec != sec) return 1;
  return 0;
}

static int test_clock_settime(struct timespec *tv)
{
  int result = clock_settime(CLOCK_REALTIME, tv);
  if (result) return 1;
  return 0;
}

void test_clock_gettime_settime(void)
{
  struct timespec tv0, tv;

  test_begin("Get clock real time");
  memset(&tv0, 0x55, sizeof(tv0));
  int result = clock_gettime(CLOCK_REALTIME, &tv0);
  if (result) test_failure(); else test_success();

  test_begin("Get clock resolution");
  tv.tv_sec = 0;
  tv.tv_nsec = 0;
  result = test_clock_getres(&tv, 0);
  if (result) test_failure(); else test_success();

  test_begin("Set clock real time to Y2038 minus 60 seconds");
  tv.tv_sec = 0x7FFFFFFF;
  tv.tv_sec -= 59;
  tv.tv_nsec = 0;
  result = test_clock_settime(&tv);
  if (result) test_failure(); else test_success();
  
  if (result == 0)
  {
    test_begin("Get clock real time");
	result = test_clock_gettime(&tv, tv.tv_sec);
    if (result) test_failure(); else test_success();
  }

  test_begin("Set clock real time to Y2038 plus 60 seconds");
  tv.tv_sec = 0x7FFFFFFF;
  tv.tv_sec += 61;
  tv.tv_nsec = 0;
  result = test_clock_settime(&tv);
  if (result) test_failure(); else test_success();

  if (result == 0)
  {
    test_begin("Get clock real time");
	result = test_clock_gettime(&tv, tv.tv_sec);
    if (result) test_failure(); else test_success();
  }

  test_begin("Restore clock real time");
  result = clock_settime(CLOCK_REALTIME, &tv0);
  if (result) test_failure(); else test_success();
}
