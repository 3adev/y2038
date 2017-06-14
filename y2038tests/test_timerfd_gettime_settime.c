#include <signal.h>
#include <sys/timerfd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#include "tests.h"

static int test_timerfd_gettime(int fd, struct itimerspec
  *value)
{
  int result = timerfd_gettime(fd, value);
  if (result) return 1;
  return 0;
}

static int test_timerfd_settime(int fd, const struct itimerspec
  *value)
{
  int result = timerfd_settime(fd, 0, value, NULL);
  if (result) return 1;
  return 0;
}

void test_timerfd_gettime_settime(void)
{
  int fd;
  struct sigevent sevp;

  test_begin("Call timerfd_create()");
  sevp.sigev_notify = SIGEV_NONE;
  int result = timerfd_create(CLOCK_REALTIME, 0);
  if (result==-1) test_failure(); else 
  {
	  test_success();
	  fd = result;
  }

  test_begin("Call timerfd_settime()");
  struct itimerspec settings = { { 2, 0 }, { 2, 0 } };
  result = test_timerfd_settime(fd, &settings);
  if (result) test_failure(); else test_success();

  test_begin("Call timerfd_gettime() before delay");
  struct itimerspec value1;
  result = test_timerfd_gettime(fd, &value1);
  if (result) test_failure(); else test_success();

  test_begin("Sleep for 1 000 000 microseconds");
  result = usleep(1000000);
  if (result) test_failure(); else test_success();

  test_begin("Call timerfd_gettime() after delay");
  struct itimerspec value2;
  result = test_timerfd_gettime(fd, &value2);
  if (result) test_failure(); else test_success();

  test_begin("Check difference between calls to test_timerfd_gettime()");
  int64_t diff = value1.it_value.tv_sec - value2.it_value.tv_sec;
  diff *= 1000000000;
  diff += value1.it_value.tv_nsec - value2.it_value.tv_nsec;
  diff -= 1000000000;
  diff = abs(diff);
  if (diff > 100000000) test_failure(); else test_success();

  test_begin("Call close() on timer fd");
  result = close(fd);
  if (result) test_failure(); else test_success();
}
