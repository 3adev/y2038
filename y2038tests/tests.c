#include "tests.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

static int test_failures;
static int test_successes;
static const char *test_desc;

void tests_init(void)
{
  test_successes =0;
  test_failures =0;
  printf("Beginning tests\n");
  printf("----------------------------------------------------\n");
}

static int test_underway;

void test_begin(const char *description)
{
  test_desc = description;
  test_underway = 1;
}

void test_failure(int print_errno, const char *fmt, ...)
{
  va_list ap;
  printf("FAILED -- %s ", test_desc);
  va_start(ap, fmt);
  vprintf(fmt, ap);
  va_end(ap);
  if (print_errno) printf(" (errno= %d / %s)", errno, strerror(errno));
  printf("\n");
  if (test_underway) test_failures++;
  test_underway = 0;
}

void test_success(void)
{
  printf("ok     -- %s\n", test_desc);
  if (test_underway) test_successes++;
  test_underway = 0;
}

void tests_report(void)
{
  printf("----------------------------------------------------\n");
  printf("Test succeeded: %d\n", test_successes);
  printf("Test failed: %d\n", test_failures);
}
