#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "tests.h"

static void test_ctime_call(
  const time_t *timep,
  const char *expected_result)
{
  const char *result = ctime(timep);
  if (!result)
    test_failure(1, "ctime returned NULL");
  else if (strcmp(result, expected_result))
    test_failure(0, "ctime returned '%s' instead of '%s'", result, expected_result);
  else
    test_success();
}

void test_ctime(void)
{
  int result;
  time_t t;
 
  test_begin("Check that ctime(\"Thu Jan  1 00:00:00 1970\") == 0");
  t = 0; test_ctime_call(&t, "Thu Jan  1 00:00:00 1970\n");

  test_begin("Check that ctime(\"Tue Jan 19 03:14:07 2038\") == 0x7FFFFFFF");
  t = 0x7fffffff; test_ctime_call(&t, "Tue Jan 19 03:14:07 2038\n");

  test_begin("Check that ctime(\"Tue Jan 19 03:14:08 2038\") == 0x80000000");
  t = 0x7fffffff; t++; test_ctime_call(&t, "Tue Jan 19 03:14:08 2038\n");
}
