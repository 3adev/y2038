#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "tests.h"

static int test_difftime_call(
  time_t time1,
  time_t time0,
  double expected_result)
{
  double result = difftime(time1, time0);
  if (result != expected_result) return 1;
  return 0;
}

void test_difftime(void)
{
  time_t time1, time0;
  int result;

  test_begin("Check that difftime(+1800, -1800) == 3600");
  time1 = +1800; time0 = -1800; result = test_difftime_call(time1, time0, +3600.0);
  if (result) test_failure(); else test_success();

  test_begin("Check that difftime(-1800, +1800) == -3600");
  time1 = -1800; time0 = +1800; result = test_difftime_call(time1, time0, -3600.0);
  if (result) test_failure(); else test_success();

  test_begin("Check that difftime(Y2038+1799, Y2038-1801) == 3600");
  time1 = time0 = 0x7FFFFFFF; time1 += 1800; time0 -= 1800; result = test_difftime_call(time1, time0, +3600.0);
  if (result) test_failure(); else test_success();

  test_begin("Check that difftime(Y2038+1800, Y2038-1800) == 3600");
  time1 = time0 = 0x80000000; time1 += 1800; time0 -= 1800; result = test_difftime_call(time1, time0, +3600.0);
  if (result) test_failure(); else test_success();

  test_begin("Check that difftime(Y2038-1800, Y2038+1800) == -3600");
  time1 = time0 = 0x80000000; time1 -= 1800; time0 += 1800; result = test_difftime_call(time1, time0, -3600.0);
  if (result) test_failure(); else test_success();

  test_begin("Check that difftime(Y2038-1801, Y2038-1799) == -3600");
  time1 = time0 = 0x7FFFFFFF; time1 -= 1800; time0 += 1800; result = test_difftime_call(time1, time0, -3600.0);
  if (result) test_failure(); else test_success();
}
