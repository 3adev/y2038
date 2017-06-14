#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "tests.h"

static int test_fstatat_call(
  const char  *pathname,
  struct stat *buf)
{
  int result = fstatat(0, pathname, buf, 0);
  return result;
}

void test_fstatat(void)
{
  int result;
  struct stat buf;

  test_begin("Call fstatat() on /etc/init.d/rcS");
  result = test_fstatat_call("/etc/init.d/rcS", &buf);
  if (result) test_failure(); else test_success();
}
