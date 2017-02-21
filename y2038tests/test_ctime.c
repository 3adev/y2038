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

static int test_ctime_call(
  const time_t *timep,
  const char *expected_result)
{
  const char *result = ctime(timep);
  if (!result)
  {
    printf("ctime(" FMTD ") returned NULL, errno = %d (%s)\n",
      *timep, errno, strerror(errno));
      return 1;
  }
  if (strcmp(result, expected_result))
  {
    printf("ctime(" FMTD ") returned \"%s\" instead of \"%s\"\n",
      *timep, result, expected_result);
      return 1;
  }
  return 0;
}

void test_ctime(int *tests_run, int *tests_fail)
{
  int result;
  time_t t;
  t = 0; result = test_ctime_call(&t, "Thu Jan  1 00:00:00 1970\n");
  (*tests_run)++; (*tests_fail) += result;
  t = 0x7fffffff; result = test_ctime_call(&t, "Tue Jan 19 03:14:07 2038\n");
  (*tests_run)++; (*tests_fail) += result;
  t = 0x7fffffff; t++; result = test_ctime_call(&t, "Tue Jan 19 03:14:08 2038\n");
  (*tests_run)++; (*tests_fail) += result;
}
