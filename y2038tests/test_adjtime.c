#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "tests.h"

void test_adjtime(void)
{
  struct timespec now, then;
  struct timeval delta = { 0, 1000 };
  struct timeval old;
  int64_t diff;
  int result;

  test_begin("Get current real time before adjtime()");
  result = clock_gettime(CLOCK_REALTIME, &then);
  if (result) test_failure(1, "clock_gettime returned %d", result); else test_success();

  test_begin("Adjusting time forward by 1 ms");
  result = adjtime(&delta, &old);
  if (result)
    test_failure(1, "adjtime returned %d", result);
  else
    test_success();
  
  test_begin("Get current real time after adjtime()");
  result = clock_gettime(CLOCK_REALTIME, &now);
  if (result) test_failure(1, "clock_gettime returned %d", result); else test_success();

  test_begin("Check that adjtime() did not cause more than a few ms adjustment");
  diff = now.tv_sec-then.tv_sec;
  diff *= 1000000000;
  diff += now.tv_nsec-then.tv_nsec;
  if (diff > 10000000) // 10 ms
    test_failure(0, "adjtime drifted by %lld, more than 10000000", diff);
  else
    test_success();
}
