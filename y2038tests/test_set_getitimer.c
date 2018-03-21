#include <sys/time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#include "tests.h"

static int test_getitimer(struct itimerval *value)
{
  return getitimer(ITIMER_REAL, value);
}

static int test_setitimer(const struct itimerval
  *value)
{
  return setitimer(ITIMER_REAL, value, NULL);
}

void test_set_getitimer(void)
{
  test_begin("Setting real time interval timer to 2 seconds");
  struct itimerval settings = { { 2, 0 }, { 2, 0 } };
  int result = test_setitimer(&settings);
  if (result) test_failure(1, "setitimer returned %d", result); else test_success();

  test_begin("Getting real time interval timer");
  struct itimerval value1;
  result = test_getitimer(&value1);
  if (result) test_failure(1, "getitimer returned %d", result); else test_success();

  usleep(1000000);

  test_begin("Getting real time interval timer 1 second later");
  struct itimerval value2;
  result = test_getitimer(&value2);
  if (result) test_failure(1, "getitimer returned %d", result); else test_success();

  test_begin("Checking real time interval timer elapsed time");
  int64_t diff = value1.it_value.tv_sec - value2.it_value.tv_sec;
  diff *= 1000000;
  diff += value1.it_value.tv_usec - value2.it_value.tv_usec;
  diff -= 1000000;
  diff = abs(diff);
  if (diff > 100000) /* 1/10 sec */
  {
    int64_t s = diff / 1000000;
    int64_t n = diff % 1000000;
    test_failure(0, "real time interval timer lasted %lld.%09lld second instead of 1.0", s, n);
  }
  else
    test_success();
}
