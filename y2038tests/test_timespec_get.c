#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "tests.h"

void test_timespec_get(void)
{
  struct timespec then, now;
  time_t diff;

  memset(&then, 0x55, sizeof(then));
  memset(&now, 0x55, sizeof(now));

  test_begin("Get current real time before timespec_get()");
  int result = clock_gettime(CLOCK_REALTIME, &then);
  if (result) test_failure(1, "clock_gettime returned %d", result); else test_success();

  test_begin("Call timespec_get(&now, TIME_UTC)");
  result = timespec_get(&now, TIME_UTC);
  if (result != TIME_UTC) test_failure(1, "timespec_get returned %d", result); else test_success();

  test_begin("Check that timespec_get() returned real time");
  diff = now.tv_sec-then.tv_sec;
  diff *= 1000000000;
  diff += now.tv_nsec-then.tv_nsec;
  diff %= 1000000000;
  if (diff > 100000000) test_failure(0, "timespec_get drifted by %lld, more than 100000000", (long long) diff); else test_success();
}
