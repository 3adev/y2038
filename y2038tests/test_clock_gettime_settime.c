#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <inttypes.h>
#include <asm/unistd.h>

#include "id_kernel.h"
#include "id_glibc.h"

#if defined _TIME_BITS && _TIME_BITS == 64
#define FMTD "%lld"
#else
#define FMTD "%ld"
#endif

static int test_clock_gettime(struct timespec *tv, time_t sec)
{
  int result = clock_gettime(CLOCK_REALTIME, tv);
  if (result)
  {
    printf("clock_gettime() returned %d (errno %d '%s')\n",
      result, errno, strerror(errno));
    return 1;
  }
  if (tv->tv_sec != sec)
  {
    printf("clock_gettime() returned tv_sec = " FMTD " instead of "
      FMTD "\n", tv->tv_sec, sec);
    return 1;
  }
  return 0;
}

static int test_clock_settime(struct timespec *tv)
{
  int result = clock_settime(CLOCK_REALTIME, tv);
  if (result)
  {
	  printf("clock_settime(" FMTD "," FMTD ") returned %d (errno %d '%s')\n",
        tv->tv_sec, tv->tv_nsec, result, errno, strerror(errno));
	  return 1;
  }
  return 0;
}

void test_clock_gettime_settime(int *tests_run, int *tests_fail)
{
  struct timespec tv;

  memset(&tv, 0x55, sizeof(tv));
  int result = clock_gettime(CLOCK_REALTIME, &tv);
  (*tests_run)++;
  if (result)
  {
    printf("clock_gettime() returned %d (errno %d '%s')\n",
      result, errno, strerror(errno));
    (*tests_fail)++;
  }

  tv.tv_sec = 0x7FFFFFFF;
  tv.tv_sec -= 59;
  tv.tv_nsec = 0;
  result = test_clock_settime(&tv);
  if (result == 0) result = test_clock_gettime(&tv, tv.tv_sec);
  (*tests_run)++; (*tests_fail) += result;

  tv.tv_sec = 0x7FFFFFFF;
  tv.tv_sec += 61;
  tv.tv_nsec = 0;
  result = test_clock_settime(&tv);
  if (result == 0) result = test_clock_gettime(&tv, tv.tv_sec);
  (*tests_run)++; (*tests_fail) += result;
}
