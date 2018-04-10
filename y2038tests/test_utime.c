#include <utime.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "tests.h"

static void test_utime_call(const char *name, const struct utimbuf *t)
{
  int result = utime(name, t);
  if (result)
    test_failure(1, "utime('%s', a=%lld m=%lld) returned %d", name, (long long) (t->actime), (long long) (t->modtime), result);
  else
    test_success();
}

void test_utime(void)
{
  test_begin("Creating /utime.null");
  int fd1 = open("/utime.null", O_CREAT);
  if (fd1 < 0)
  {
    test_failure(1, "open('/utime.null') returned %d", fd1);
    return;
  }
  close(fd1);
  test_success();

  test_begin("Call utimes(NULL) on /utime.null");
  test_utime_call("/utime.null", NULL);

  test_begin("Creating /utime.n2038");
  fd1 = open("/utime.n2038", O_CREAT);
  if (fd1 < 0)
  {
    test_failure(1, "open('/utime.n2038') returned %d", fd1);
    return;
  }
  close(fd1);
  test_success();

  test_begin("Call utime(Y2038-2, Y2038-1) on /utime.n2038");
  const struct utimbuf t1 = { 0x7FFFFFFE, 0x7FFFFFFF };
  test_utime_call("/utime.n2038", &t1);

  test_begin("Check stat of /utime.n2038");
  struct stat st1;
  int result = stat("/utime.n2038", &st1);
  if (result)
    test_failure(1, "stat('/utime.n2038') returned %d", result);
  else if (st1.st_atim.tv_sec != t1.actime || st1.st_mtim.tv_sec != t1.modtime)
  {
    test_failure(0, "utime returned a=%lld m=%lld, expected a=%lld m=%lld",
        (long long int) st1.st_atim.tv_sec,
        (long long int) st1.st_mtim.tv_sec,
        (long long int) t1.actime,
        (long long int) t1.modtime);
  }
  else
    test_success();

  test_begin("Creating /utime.y2038");
  fd1 = open("/utime.y2038", O_CREAT);
  if (fd1 < 0)
  {
    test_failure(1, "open('/utime.y2038') returned %d", fd1);
    return;
  }
  close(fd1);
  test_success();

  test_begin("Call utime(Y2038+1, Y2038+2) on /utime.y2038");
  const struct utimbuf t2 = { 0x80000001ull, 0x80000002ull };
  test_utime_call("/utime.y2038", &t2);

  test_begin("Check stat of /utime.y2038");
  struct stat st2;
  result = stat("/utime.y2038", &st2);
  if (result)
    test_failure(1, "stat returned %d", result);
  else if (st2.st_atim.tv_sec != t2.actime || st2.st_mtim.tv_sec != t2.modtime)
  {
    test_failure(0, "utime returned a=%lld m=%lld, expected a=%lld m=%lld",
        (long long int) st2.st_atim.tv_sec,
        (long long int) st2.st_mtim.tv_sec,
        (long long int) t2.actime,
        (long long int) t2.modtime);
  }
  else test_success();
}
