#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "tests.h"

static int test_fstat_call(
  int fd,
  struct stat *buf)
{
  return fstat(fd, buf);
}

void test_fstat(void)
{
  int result;
  struct stat buf;
  int fd = open("/etc/init.d/rcS", O_RDONLY);

  test_begin("Call fstat() on /etc/init.d/rcS");
  result = test_fstat_call(fd, &buf);
  if (result) test_failure(1, "fstat('/etc/init.d/rcS') returned %d", result); else test_success();

  close(fd);
}
