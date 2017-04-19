#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "id_kernel.h"
#include "id_glibc.h"

#if defined _TIME_BITS && _TIME_BITS == 64
#define FMTD "%lld"
#else
#define FMTD "%ld"
#endif

static int test_futimens_call(const char *name, const struct timespec t[2])
{
  int result = 0, fd = open(name, O_CREAT);
  if (fd == -1)
  {
    printf("futimens_test could not create %s\n", name);
    return -1;
  }
  if (t!=NULL)
  {
    result = futimens(fd, t);
    if (result != 0)
    {
      printf("futimens(\"%s\"...) returned %d (errno %d -- '%s'))\n",
        name, result, errno, strerror(errno));
    }
  }
  close(fd);
  return (result!=0);
}

void test_futimens(int *tests_run, int *tests_fail)
{
  const struct timespec t[2] = { { 0x80000000ull, 0 },  { 0x80000120ull, 0 } };
  int result = test_futimens_call("/futimens.ref", NULL);
  (*tests_run)++; (*tests_fail) += (result != 0);
  result = test_futimens_call("/futimens.tst", t);
  (*tests_run)++; (*tests_fail) += (result != 0);
}
