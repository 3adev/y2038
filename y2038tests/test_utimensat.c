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

static int test_utimensat_call(int dirfd, const char *name, const struct
  timespec t[2], int flags)
{
  close(open(name, O_CREAT));
  int result = utimensat(dirfd, name, t, flags);
  if (result != 0)
  {
    printf("utimensat(\"%s\"...) returned %d (errno %d -- '%s'))\n",
      name, result, errno, strerror(errno));
  }
  return (result!=0);
}

void test_utimensat(int *tests_run, int *tests_fail)
{
  
  const struct timespec t[2] = { { 0x80000000ull, 0 },  { 0x80000120ull, 0 } };
  int result = test_utimensat_call(0, "/utimensat.ref", NULL, 0);
  (*tests_run)++; (*tests_fail) += (result != 0);
  result = test_utimensat_call(0, "/utimensat.tst", t, 0);
  (*tests_run)++; (*tests_fail) += (result != 0);
}
