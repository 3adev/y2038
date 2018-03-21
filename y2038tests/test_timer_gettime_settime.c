#include <signal.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#include "tests.h"

static int test_timer_gettime(timer_t timerid, struct itimerspec
  *value)
{
  return timer_gettime(timerid, value);
}

static int test_timer_settime(timer_t timerid, const struct itimerspec
  *value)
{
  return timer_settime(timerid, 0, value, NULL);
}

void test_timer_gettime_settime(void)
{
  timer_t timerid;
  struct sigevent sevp;

  test_begin("Call timer_create()");
  sevp.sigev_notify = SIGEV_NONE;
  int result = timer_create(CLOCK_REALTIME, &sevp, &timerid);
  if (result) test_failure(1, "timer_create returned %d", result); else test_success();

  test_begin("Call timer_settime()");
  struct itimerspec settings = { { 2, 0 }, { 2, 0 } };
  result = test_timer_settime(timerid, &settings);
  if (result) test_failure(1, "timer_settime returned %d", result); else test_success();

  test_begin("Call timer_gettime() before sleep");
  struct itimerspec value1;
  result = test_timer_gettime(timerid, &value1);
  if (result) test_failure(1, "timer_gettime returned %d", result); else test_success();

  test_begin("Sleeping for 1 second");
  result = sleep(1);
  if (result) test_failure(1, "sleep returned %d", result); else test_success();

  test_begin("Call timer_gettime() after sleep");
  struct itimerspec value2;
  result = test_timer_gettime(timerid, &value2);
  if (result) test_failure(1, "timer_gettime returned %d", result); else test_success();

  test_begin("Check difference between timer_gettime() calls");
  int64_t diff = value1.it_value.tv_sec - value2.it_value.tv_sec;
  diff *= 1000000000;
  diff += value1.it_value.tv_nsec - value2.it_value.tv_nsec;
  diff -= 1000000000;
  diff = abs(diff);
  if (diff > 100000000) test_failure(0, "timer drift is %lld, more than 100000000", diff); else test_success();

  test_begin("Call timer_delete()");
  result = timer_delete(timerid);
  if (result) test_failure(1, "timer_delete returned %d", result); else test_success();
}
