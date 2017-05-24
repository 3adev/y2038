#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#if defined _TIME_BITS && _TIME_BITS == 64
#define FMTD "%lld"
#else
#define FMTD "%ld"
#endif

static int test_fstat_call(
  int fd,
  struct stat *buf)
{
  int result = fstat(fd, buf);
  if (result)
  {
    printf("fstat(%d) returned %d, errno = %d (%s)\n",
      fd, result, errno, strerror(errno));
  }
  return result;
}

void test_fstat(int *tests_run, int *tests_fail)
{
  int result;
  struct stat buf;
  int fd = open("/etc/init.d/rcS", O_RDONLY);
  result = test_fstat_call(fd, &buf);
  close(fd);
  (*tests_run)++; (*tests_fail) += (result!=0);
}
