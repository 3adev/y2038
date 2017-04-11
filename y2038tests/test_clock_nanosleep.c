#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "id_kernel.h"
#include "id_glibc.h"

#if defined _TIME_BITS && _TIME_BITS == 64
#define FMTD "%lld"
#else
#define FMTD "%ld"
#endif

void test_clock_nanosleep(int *tests_run, int *tests_fail)
{
  struct timespec now, then, duration = { 1, 0 };
  time_t diff;
  int result_then, result, result_now;

  result_then = clock_gettime(CLOCK_REALTIME, &then);
  result = clock_nanosleep(CLOCK_REALTIME, 0 /* relative */,
    &duration, NULL);
  result_now = clock_gettime(CLOCK_REALTIME, &now);
  (*tests_run) += 3;

  if (result_then)
  {
    printf("clock_gettime() returned %d (errno %d '%s')\n",
      result_then, errno, strerror(errno));
    (*tests_fail)++;
  }

  if (result)
  {
    printf("clock_nanosleep() returned %d (errno %d '%s')\n",
      result, errno, strerror(errno));
    (*tests_fail)++;
  }

  if (result_now)
  {
    printf("clock_gettime() returned %d (errno %d '%s')\n",
      result, errno, strerror(errno));
    (*tests_fail)++;
  }

  diff = now.tv_sec-then.tv_sec;
  diff *= 1000000000;
  diff += now.tv_nsec-then.tv_nsec;
  diff = abs(diff - 1000000000); /* how far from 1 second? */
  (*tests_run) += 3;

  if (diff > 10000000) /* more than 10 ms ( = 10000000 ns )*/
  {
    printf("clock_nanosleep() was in error by " FMTD " ns\n", diff);
    (*tests_fail)++;
  }
}
