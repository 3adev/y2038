#include <signal.h>
#include <sys/timerfd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#include "id_kernel.h"
#include "id_glibc.h"

#if defined _TIME_BITS && _TIME_BITS == 64
#define FMTD "%lld"
#else
#define FMTD "%ld"
#endif

static int test_timerfd_gettime(int fd, struct itimerspec
  *value)
{
  int result = timerfd_gettime(fd, value);
  if (result)
  {
    printf("timerfd_gettime() returned %d (errno %d '%s')\n",
      result, errno, strerror(errno));
    return 1;
  }
  return 0;
}

static int test_timerfd_settime(int fd, const struct itimerspec
  *value)
{
  int result = timerfd_settime(fd, 0, value, NULL);
  if (result)
  {
    printf("timerfd_settime(" FMTD ".%06ld, " FMTD ".%06ld) returned %d "
           "(errno %d '%s')\n",
           value->it_value.tv_sec, value->it_value.tv_nsec,
           value->it_interval.tv_sec, value->it_interval.tv_nsec,
           result, errno, strerror(errno));
    return 1;
  }
  return 0;
}

void test_timerfd_gettime_settime(int *tests_run, int *tests_fail)
{
  int fd;
  struct sigevent sevp;

  sevp.sigev_notify = SIGEV_NONE;
  int result = timerfd_create(CLOCK_REALTIME, 0);
  (*tests_run)++;
  if (result == -1)
    (*tests_fail)++;
  else
    fd = result;

  struct itimerspec settings = { { 2, 0 }, { 2, 0 } };
  result = test_timerfd_settime(fd, &settings);
  (*tests_run)++;
  if (result)
    (*tests_fail)++;

  struct itimerspec value1;
  result = test_timerfd_gettime(fd, &value1);
  (*tests_run)++;
  if (result)
    (*tests_fail)++;

  usleep(1000000);

  struct itimerspec value2;
  result = test_timerfd_gettime(fd, &value2);
  (*tests_run)++;
  if (result)
    (*tests_fail)++;

  int64_t diff = value1.it_value.tv_sec - value2.it_value.tv_sec;
  diff *= 1000000000;
  diff += value1.it_value.tv_nsec - value2.it_value.tv_nsec;
  diff -= 1000000000;
  diff = abs(diff);
  (*tests_run)++;
  if (diff > 100000000) /* 1/10 sec */
  {
    int64_t s = diff / 1000000000;
    int64_t n = diff % 1000000000;
    printf("timerfd lasted %lld.%09lld second instead of 1.0\n", s, n);
    (*tests_fail)++;
  }
}
