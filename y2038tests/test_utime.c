#include <utime.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "tests.h"

static int test_utime_call(const char *name, const struct utimbuf *t)
{
  int result = utime(name, t);
  return (result!=0);
}

void test_utime(void)
{
  test_begin("Creating /utime.null");
  int fd1 = open("/utime.null", O_CREAT);
  if (fd1 < 0)
  {
    test_failure();
    return;
  }
  close(fd1);
  test_success();

  test_begin("Call utimes(NULL) on /utime.null");
  int result = test_utime_call("/utime.null", NULL);
  if (result) test_failure(); else test_success();

  test_begin("Creating /utime.n2038");
  fd1 = open("/utime.n2038", O_CREAT);
  if (fd1 < 0)
  {
    test_failure();
    return;
  }
  close(fd1);
  test_success();

  test_begin("Call utime(Y2038-2, Y2038-1) on /utime.n2038");
  const struct utimbuf t1 = { 0x7FFFFFFE, 0x7FFFFFFF };
  result = test_utime_call("/utime.n2038", &t1);
  if (result) test_failure(); else test_success();

  test_begin("Check stat of /utime.n2038");
  struct stat st1;
  result = stat("/utime.n2038", &st1);
  if (result)
    test_failure();
  else if (st1.st_atim.tv_sec != t1.actime || st1.st_ctim.tv_sec != t1.modtime)
  {
    test_failure(); 
    printf("utime %lld : %lld - %lld : %lld\n",
        (long long int) st1.st_atim.tv_sec,
        (long long int) t1.actime,
        (long long int) st1.st_ctim.tv_sec,
        (long long int) t1.modtime);
  }
  else
    test_success();

  test_begin("Creating /utime.y2038");
  fd1 = open("/utime.y2038", O_CREAT);
  if (fd1 < 0)
  {
    test_failure();
    return;
  }
  close(fd1);
  test_success();

  test_begin("Call utime(Y2038+1, Y2038+2) on /utime.y2038");
  const struct utimbuf t2 = { 0x80000001ull, 0x80000002ull };
  result = test_utime_call("/utime.y2038", &t2);
  if (result) test_failure(); else test_success();

  test_begin("Check stat of /utime.y2038");
  struct stat st2;
  result = stat("/utime.y2038", &st2);
  if (result)
    test_failure();
  else if (st2.st_atim.tv_sec != t2.actime || st2.st_ctim.tv_sec != t2.modtime)
  {
    test_failure(); 
    printf("utime %lld : %lld - %lld : %lld\n",
        (long long int) st2.st_atim.tv_sec,
        (long long int) t2.actime,
        (long long int) st2.st_ctim.tv_sec,
        (long long int) t2.modtime);
  }
  else
    test_success();
}
