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
  long double expected_result)
{
  long double result = difftime(time1, time0);
  if (result != expected_result)
  {
    printf("difftime(" FMTD ", " FMTD ") returned %Lg instead of %Lg\n",
      time1, time0, result, expected_result);
      return 1;
  }
  return 0;
}

void test_difftime(int *tests_run, int *tests_fail)
{
  int result = test_difftime_call(3600, 7200, -3600.0);
  (*tests_run)++; (*tests_fail) += result;
  result = test_difftime_call(7200, 3600, 3600.0);
  (*tests_run)++; (*tests_fail) += result;
  result = test_difftime_call(0x80000000-1800, 0x80000000+1800, -3600.0);
  (*tests_run)++; (*tests_fail) += result;
  result = test_difftime_call(0x80000000+1800, 0x80000000-1800, +3600.0);
  (*tests_run)++; (*tests_fail) += result;
}
