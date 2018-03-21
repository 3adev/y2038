#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "tests.h"

static int test_lstat_call(
  const char  *pathname,
  struct stat *buf)
{
  int result = lstat(pathname, buf);
  return result;
}

void test_lstat(void)
{
  int result;
  struct stat buf;

  test_begin("Call lstat() on /etc/init.d/rcS");
  result = test_lstat_call("/etc/init.d/rcS", &buf);
  if (result) test_failure(1, "lstat('/etc/init.d/rcS') returned %d", result); else test_success();
}
