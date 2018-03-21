#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "tests.h"

static void test_futimens_call(const char *name, const struct timespec t[2])
{
  int result = 0, fd = open(name, O_CREAT);
  if (fd == -1)
  {
    test_failure(1, "open('%s', O_CREAT) returned %d", name, fd);
    return;
  }
  int c = close(fd);
  if (c)
  {
    test_failure(1, "close returned %d", c);
    return;
  }
  fd = open(name, O_RDWR);
  if (fd == -1)
  {
    test_failure(1, "open('%s', O_RDWR) returned %d", name, fd);
    return;
  }
  if (t==NULL)
    test_success();
  else
  {
    result = futimens(fd, t);
    if (result)
      test_failure(1, "futimens('%s'...) returned %d", name, result);
    else
      test_success();
  }
  close(fd);
}

void test_futimens(void)
{
  test_begin("Call futimens(NULL) on /futimens.null");
  test_futimens_call("/futimens.null", NULL);

  test_begin("Call futimens(Y2038-2, Y2038-1) on /futimens.n2038");
  const struct timespec t1[2] = { { 0x7FFFFFFE, 0 },  { 0x7FFFFFFF, 0 } };
  test_futimens_call("/futimens.n2038", t1);

  test_begin("Check stat of /futimens.n2038");
  struct stat st1;
  int result = stat("/futimens.n2038", &st1);
  if (result) test_failure(1, "stat('/futimens.n2038') returned %d", result); else test_success();

  test_begin("Call futimens(Y2038+1, Y2038+2) on /futimens.y2038");
  const struct timespec t2[2] = { { 0x80000001ull, 0 },  { 0x80000002ull, 0 } };
  test_futimens_call("/futimens.y2038", t2);

  test_begin("Check stat of /futimens.y2038");
  struct stat st2;
  result = stat("/futimens.y2038", &st2);
  if (result) test_failure(1, "stat('/futimens.y2038') returned %d", result); else test_success();
}
