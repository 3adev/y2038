#include <time.h>
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

static int test_difftime_call(
  time_t time1,
  time_t time0,
  double expected_result)
{
  double result = difftime(time1, time0);
  if (result != expected_result)
  {
    printf("difftime(" FMTD ", " FMTD ") returned %g instead of %g\n",
      time1, time0, result, expected_result);
      return 1;
  }
  return 0;
}

void test_difftime(int *tests_run, int *tests_fail)
{
  time_t time1, time0;
  int result;
  time1 = +1800; time0 = -1800; result = test_difftime_call(time1, time0, +3600.0);
  (*tests_run)++; (*tests_fail) += result;
  time1 = -1800; time0 = +1800; result = test_difftime_call(time1, time0, -3600.0);
  (*tests_run)++; (*tests_fail) += result;
  time1 = time0 = 0x7FFFFFFF; time1 += 1800; time0 -= 1800; result = test_difftime_call(time1, time0, +3600.0);
  (*tests_run)++; (*tests_fail) += result;
  time1 = time0 = 0x80000000; time1 += 1800; time0 -= 1800; result = test_difftime_call(time1, time0, +3600.0);
  (*tests_run)++; (*tests_fail) += result;
  time1 = time0 = 0x80000000; time1 -= 1800; time0 += 1800; result = test_difftime_call(time1, time0, -3600.0);
  (*tests_run)++; (*tests_fail) += result;
  time1 = time0 = 0x7FFFFFFF; time1 -= 1800; time0 += 1800; result = test_difftime_call(time1, time0, -3600.0);
  (*tests_run)++; (*tests_fail) += result;
}
