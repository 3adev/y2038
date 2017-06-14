#include "tests.h"

#include <stdio.h>

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

void test_begin(const char *description)
{
  test_desc = description;
}

void test_failure(void)
{
  printf("FAILED -- %s\n", test_desc);
  test_failures++;
}

void test_success(void)
{
  printf("ok     -- %s\n", test_desc);
  test_successes++;
}

void tests_report(void)
{
  printf("----------------------------------------------------\n");
  printf("Test succeeded: %d\n", test_successes);
  printf("Test failed: %d\n", test_failures);
}
