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
static int test_failed;
static int test_succeeded;

void test_end(void)
{
  if (test_underway)
  {
    printf(" ??? No test result was reported\n");
    test_underway = 0;
  }
  else if (test_failed)
  {
    printf(" FAILED -- %s\n", test_desc);
    test_failures++;
    test_failed = 0;
  }
  else if (test_succeeded)
  {
    test_successes++;
    printf(" ok     -- %s\n", test_desc);
    test_succeeded = 0;
  }
  else
  {
    printf(" ??? No test was underway\n");
  }
}

void test_begin(const char *description)
{
  if (test_underway || test_failed || test_succeeded) test_end();
  test_desc = description;
  test_underway = 1;
  printf("Testing -- %s\n", test_desc);
}

void test_failure(int print_errno, const char *fmt, ...)
{
  va_list ap;
  printf(" --- ");
  va_start(ap, fmt);
  vprintf(fmt, ap);
  va_end(ap);
  if (print_errno) printf(" (errno= %d / %s)", errno, strerror(errno));
  printf("\n");
  test_failed = 1;
  test_underway = 0;
}

void test_success(void)
{
  test_succeeded = 1;
  test_underway = 0;
}

void test_run(void (*test_function)(void))
{
  test_function();
  test_end();
}

void tests_report(void)
{
  printf("----------------------------------------------------\n");
  printf("Test succeeded: %d\n", test_successes);
  printf("Test failed: %d\n", test_failures);
}
