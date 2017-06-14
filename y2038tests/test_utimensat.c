#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "tests.h"

static int test_utimensat_call(int dirfd, const char *name, const struct
  timespec t[2], int flags)
{
  close(open(name, O_CREAT));
  int result = utimensat(dirfd, name, t, flags);
  return (result!=0);
}

void test_utimensat(void)
{
  
  const struct timespec t[2] = { { 0x80000000ull, 0 },  { 0x80000120ull, 0 } };

  test_begin("test_utimensat_call(0, \"/utimensat.ref\", NULL, 0)");
  int result = test_utimensat_call(0, "/utimensat.ref", NULL, 0);
  if (result) test_failure(); else test_success();

  test_begin("test_utimensat_call(0, \"/utimensat.tst\", {{Y2038, 0},{Y2038+120s,0}}, 0)");
  result = test_utimensat_call(0, "/utimensat.tst", t, 0);
  if (result) test_failure(); else test_success();
}
