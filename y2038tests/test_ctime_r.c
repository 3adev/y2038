#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "tests.h"

static int test_ctime_r_call(
  const time_t *timep,
  const char *expected_result)
{
  char buf[26]; // that's the minimum mandated size for a ctime_r buffer
  const char *result = ctime_r(timep, buf);
  if (!result) return 1;
  if (strcmp(result, expected_result)) return 1;
  return 0;
}

void test_ctime_r(void)
{
  int result;
  time_t t;

  test_begin("Check that ctime_r(\"Thu Jan  1 00:00:00 1970\") == 0");
  t = 0; result = test_ctime_r_call(&t, "Thu Jan  1 00:00:00 1970\n");
  if (result) test_failure(); else test_success();

  test_begin("Check that ctime_r(\"Tue Jan 19 03:14:07 2038\") == 0x7FFFFFFF");
  t = 0x7fffffff; result = test_ctime_r_call(&t, "Tue Jan 19 03:14:07 2038\n");
  if (result) test_failure(); else test_success();

  test_begin("Check that ctime_r(\"Tue Jan 19 03:14:08 2038\") == 0x80000000");
  t = 0x7fffffff; t++; test_ctime_r_call(&t, "Tue Jan 19 03:14:08 2038\n");
  if (result) test_failure(); else test_success();
}
