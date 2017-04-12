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

void test_timespec_get(int *tests_run, int *tests_fail)
{
  struct timespec then, now;
  time_t diff;

  memset(&then, 0x55, sizeof(then));
  memset(&now, 0x55, sizeof(now));

  int result = clock_gettime(CLOCK_REALTIME, &then);
  (*tests_run)++;
  if (result)
  {
    printf("clock_gettime() returned %d (errno %d '%s')\n",
      result, errno, strerror(errno));
    (*tests_fail)++;
  }

  result = timespec_get(&now, TIME_UTC);
  (*tests_run)++;
  if (result != TIME_UTC)
  {
    printf("timespec_get() returned %d (errno %d '%s')\n",
      result, errno, strerror(errno));
    (*tests_fail)++;
  }

  diff = now.tv_sec-then.tv_sec;
  diff *= 1000000000;
  diff += now.tv_nsec-then.tv_nsec;
  diff %= 1000000000;

  if (diff > 1000000) /* more than 1 ms ( = 1000000 ns )*/
  {
    printf("timespec_get() was in error by " FMTD " ns\n", diff);
    (*tests_fail)++;
  }
}
