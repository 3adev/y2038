#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "tests.h"

static int test_stat_call(
  const char  *pathname,
  struct stat *buf)
{
  return stat(pathname, buf);
}

void test_stat(void)
{
  int result;
  struct stat buf;

  test_begin("Call stat() on /etc/init.d/rcS");
  result = test_stat_call("/etc/init.d/rcS", &buf);
  if (result) test_failure(1, "stat('/etc/init.d/rcS') returned %d", result); else test_success();
}
