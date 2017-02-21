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

static void dump_tm(const struct tm *tm)
{
  printf(" * tm_sec = %d\n", tm->tm_sec);
  printf(" * tm_min = %d\n", tm->tm_min);
  printf(" * tm_hour = %d\n", tm->tm_hour);
  printf(" * tm_mday = %d\n", tm->tm_mday);
  printf(" * tm_mon = %d\n", tm->tm_mon);
  printf(" * tm_year = %d\n", tm->tm_year);
  printf(" * tm_wday = %d\n", tm->tm_wday);
  printf(" * tm_yday = %d\n", tm->tm_yday);
  printf(" * tm_isdst = %d\n", tm->tm_isdst);
}

static int test_gmtime_r_call(
  const time_t *timep,
  const struct tm *expected_result)
{
  struct tm result;
  if (gmtime_r(timep, &result) == NULL)
  {
    printf("gmtime(" FMTD ") returned NULL, errno = %d (%s)\n",
      *timep, errno, strerror(errno));
    return 1;
  }
  if (result.tm_sec != expected_result->tm_sec ||
      result.tm_min != expected_result->tm_min ||
      result.tm_hour != expected_result->tm_hour ||
      result.tm_mday != expected_result->tm_mday ||
      result.tm_mon != expected_result->tm_mon ||
      result.tm_year != expected_result->tm_year ||
      result.tm_wday != expected_result->tm_wday ||
      result.tm_yday != expected_result->tm_yday ||
      result.tm_isdst != expected_result->tm_isdst)
  {
    printf("gmtime(" FMTD ") returned:\n", *timep);
    dump_tm(&result);
    printf("    instead of:\n");
    dump_tm(expected_result);
    return 1;
  }
  return 0;
}

void test_gmtime_r(int *tests_run, int *tests_fail)
{
  int result;
  struct tm tm0 = { 0, 0, 0, 1, 0, 70, 4, 0, 0 };
  time_t t = 0;
  result = test_gmtime_r_call(&t, &tm0);
  (*tests_run)++; (*tests_fail) += result;
  struct tm tmY2038 = { 7, 14, 3, 19, 0, 138, 2, 18, 0 };
  t = 0x7fffffff;
  result = test_gmtime_r_call(&t, &tmY2038);
  (*tests_run)++; (*tests_fail) += result;
  struct tm tmY2038p = { 8, 14, 3, 19, 0, 138, 2, 18, 0 };
  t++;
  result = test_gmtime_r_call(&t, &tmY2038p);
  (*tests_run)++; (*tests_fail) += result;
  t = 0x80000000;
  result = test_gmtime_r_call(&t, &tmY2038p);
  (*tests_run)++; (*tests_fail) += result;
  t--;
  result = test_gmtime_r_call(&t, &tmY2038);
  (*tests_run)++; (*tests_fail) += result;
}
