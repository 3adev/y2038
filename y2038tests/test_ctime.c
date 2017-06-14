#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "tests.h"

static int test_ctime_call(
  const time_t *timep,
  const char *expected_result)
{
  const char *result = ctime(timep);
  if (!result) return 1;
  if (strcmp(result, expected_result)) return 1;
  return 0;
}

void test_ctime(void)
{
  int result;
  time_t t;
 
  test_begin("Check that ctime(\"Thu Jan  1 00:00:00 1970\") == 0");
  t = 0; result = test_ctime_call(&t, "Thu Jan  1 00:00:00 1970\n");
  if (result) test_failure(); else test_success();

  test_begin("Check that ctime(\"Tue Jan 19 03:14:07 2038\") == 0x7FFFFFFF");
  t = 0x7fffffff; result = test_ctime_call(&t, "Tue Jan 19 03:14:07 2038\n");
  if (result) test_failure(); else test_success();

  test_begin("Check that ctime(\"Tue Jan 19 03:14:08 2038\") == 0x80000000");
  t = 0x7fffffff; t++; result = test_ctime_call(&t, "Tue Jan 19 03:14:08 2038\n");
  if (result) test_failure(); else test_success();
}
