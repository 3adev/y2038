#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "tests.h"

static int test_futimens_call(const char *name, const struct timespec t[2])
{
  int result = 0, fd = open(name, O_CREAT);
  if (fd == -1) return -1;
  if (close(fd)) return -1;
  fd = open(name, O_RDWR);
  if (fd == -1) return -1;
  if (t!=NULL) result = futimens(fd, t);
  close(fd);
  return (result!=0);
}

void test_futimens(void)
{
  test_begin("Call futimens(NULL) on /futimens.null");
  int result = test_futimens_call("/futimens.null", NULL);
  if (result) test_failure(); else test_success();

  test_begin("Call futimens(Y2038-2, Y2038-1) on /futimens.n2038");
  const struct timespec t1[2] = { { 0x7FFFFFFE, 0 },  { 0x7FFFFFFF, 0 } };
  result = test_futimens_call("/futimens.n2038", t1);
  if (result) test_failure(); else test_success();

  test_begin("Check stat of /futimens.n2038");
  struct stat st1;
  result = stat("/futimens.n2038", &st1);
  if (result) test_failure(); else test_success();

  test_begin("Call futimens(Y2038+1, Y2038+2) on /futimens.y2038");
  const struct timespec t2[2] = { { 0x80000001ull, 0 },  { 0x80000002ull, 0 } };
  result = test_futimens_call("/futimens.y2038", t2);
  if (result) test_failure(); else test_success();

  test_begin("Check stat of /futimens.y2038");
  struct stat st2;
  result = stat("/futimens.y2038", &st2);
  if (result) test_failure(); else test_success();
}
