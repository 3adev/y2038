#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#if defined _TIME_BITS && _TIME_BITS == 64
#define FMTD "%lld"
#else
#define FMTD "%ld"
#endif

static int test_lstat_call(
  const char  *pathname,
  struct stat *buf)
{
  int result = lstat(pathname, buf);
  if (result)
  {
    printf("lstat(\"%s\") returned %d, errno = %d (%s)\n",
      pathname, result, errno, strerror(errno));
  }
  return result;
}

void test_lstat(int *tests_run, int *tests_fail)
{
  int result;
  struct stat buf;
  result = test_lstat_call("/etc/init.d/rcS", &buf);
  (*tests_run)++; (*tests_fail) += (result!=0);
}
