#include <time.h>
#include <errno.h>
#include <string.h>

#include "tests.h"

static int test_time(const time_t sec)
{
  time_t t0 = time(NULL);
  double d = difftime(t0, sec);
  if (d < -1.0 || d > 1.0) return 1;
  return 0;
}

static int test_stime(const time_t sec)
{
  int result = stime(&sec);
  if (result) return 1;
  return 0;
}

void test_time_stime(void)
{
  time_t t0, t;

  test_begin("Save current time");
  t0 = time(NULL);
  if (t0==-1) test_failure(); else test_success();

  test_begin("Set time to Y2038 minus 60 seconds");
  t = 0x7FFFFFFF;
  t -= 59;
  int result = test_stime(t);
  if (result) test_failure(); else test_success();
  
  if (result == 0)
  {
    test_begin("Check time against Y2038-60s");
    result = test_time(t);
    if (result) test_failure(); else test_success();
  }

  test_begin("Set time to Y2038 plus 60 seconds");
  t = 0x7FFFFFFF;
  t += 61;
  result = test_stime(t);
  if (result) test_failure(); else test_success();

  if (result == 0)
  {
    test_begin("Check time against Y2038+60s");
    result = test_time(t);
    if (result) test_failure(); else test_success();
  }

  test_begin("Restore time");
  result = stime(&t0);
  if (result) test_failure(); else test_success();
}
