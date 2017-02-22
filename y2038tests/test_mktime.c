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

static int test_mktime_call(
  struct tm *tm,
  time_t expected_result)
{
  time_t result = mktime(tm);
  if (result == -1)
  {
    printf("mktime(\n");
    printf("    tm_sec = %d,\n", tm->tm_sec);
    printf("    tm_min = %d,\n", tm->tm_min);
    printf("    tm_hour = %d,\n", tm->tm_hour);
    printf("    tm_mday = %d,\n", tm->tm_mday);
    printf("    tm_mon = %d,\n", tm->tm_mon);
    printf("    tm_year = %d,\n", tm->tm_year);
    printf("    tm_wday = %d,\n", tm->tm_wday);
    printf("    tm_yday = %d,\n", tm->tm_yday);
    printf("    tm_isdst = %d)\n", tm->tm_isdst);
    printf("  returned -1, errno = %d (%s)\n",
      errno, strerror(errno));
    return 1;
  }
  if (result != expected_result)
  {
    printf("mktime(\n");
    printf("    tm_sec = %d,\n", tm->tm_sec);
    printf("    tm_min = %d,\n", tm->tm_min);
    printf("    tm_hour = %d,\n", tm->tm_hour);
    printf("    tm_mday = %d,\n", tm->tm_mday);
    printf("    tm_mon = %d,\n", tm->tm_mon);
    printf("    tm_year = %d,\n", tm->tm_year);
    printf("    tm_wday = %d,\n", tm->tm_wday);
    printf("    tm_yday = %d,\n", tm->tm_yday);
    printf("    tm_isdst = %d)\n", tm->tm_isdst);
    printf("  returned " FMTD " instead of " FMTD "\n",
      result, expected_result);
    return 1;
  }
  return 0;
}

void test_mktime(int *tests_run, int *tests_fail)
{
  int result;
  struct tm tm0 = { 0, 0, 0, 1, 0, 70, 4, 0, 0 };
  time_t t = 0;
  result = test_mktime_call(&tm0, t);
  (*tests_run)++; (*tests_fail) += result;
  struct tm tmY2038 = { 7, 14, 3, 19, 0, 138, 2, 18, 0 };
  t = 0x7fffffff;
  result = test_mktime_call(&tmY2038, t);
  (*tests_run)++; (*tests_fail) += result;
  struct tm tmY2038p = { 8, 14, 3, 19, 0, 138, 2, 18, 0 };
  t++;
  result = test_mktime_call(&tmY2038p, t);
  (*tests_run)++; (*tests_fail) += result;
  t = 0x80000000;
  result = test_mktime_call(&tmY2038p, t);
  (*tests_run)++; (*tests_fail) += result;
  t--;
  result = test_mktime_call(&tmY2038, t);
  (*tests_run)++; (*tests_fail) += result;
}
