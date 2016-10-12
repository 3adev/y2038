#include <stdio.h>
#include <sys/utsname.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include <asm/unistd.h>

/**
 * Print the kernel version
 *
 * @return 0 if OK
 */
static int print_kernel_version(void)
{
  struct utsname utsname;
  if (uname(&utsname) < 0)
  {
	  printf("uname() failed with error %d (%s)\n", errno, strerror(errno));
	  return 1;
  }
  printf("uname() result:\n");
  printf("- sysname      : %s\n", utsname.sysname);
  printf("- nodename     : %s\n", utsname.nodename);
  printf("- release      : %s\n", utsname.release);
  printf("- version      : %s\n", utsname.version);
  printf("- machine      : %s\n", utsname.machine);
  return 0;
}

static int print_glibc_version(void)
{
  char *confbuf;
  int conflen;

  conflen = confstr(  _CS_GNU_LIBC_VERSION, NULL, 0);
  if (!conflen)
  {
	  printf("confstr(_CS_GNU_LIBC_VERSION, NULL, 0) failed with error %d (%s)\n", errno, strerror(errno));
	  return 1;
  }


  conflen = confstr(  _CS_GNU_LIBC_VERSION, NULL, 0);
  if (!conflen)
  {
	  printf("confstr(_CS_GNU_LIBC_VERSION, NULL, 0) failed with error %d (%s)\n", errno, strerror(errno));
	  return 1;
  }

  confbuf = malloc(conflen);
  if (!confbuf)
  {
	  printf("malloc(%d) failed with error %d (%s)\n", conflen, errno, strerror(errno));
	  return 1;
  }

  if (conflen != confstr(  _CS_GNU_LIBC_VERSION, confbuf, conflen))
  {
	  printf("confstr(_CS_GNU_LIBC_VERSION, %p, %d) failed with error %d (%s)\n", confbuf, conflen, errno, strerror(errno));
	  return 1;
  }

  printf("confstr() result:\n");
  printf("- LIBC version: %s\n", confbuf);
  return 0;
}

static int test_clock_gettime(struct timespec *tv)
{
  int result = clock_gettime(CLOCK_REALTIME, tv);
  printf("clock_gettime() returned %d\n", result);
  if (result)
  {
	  printf("- errno = %d\n", errno);
	  printf("- strerror(errno) =\"%s\"\n", strerror(errno));
	  return 1;
  }
  else
  {
    printf("- tv.tv_sec = %llu (hex: %llx)\n", (long long) tv->tv_sec, (long long) tv->tv_sec);
    printf("- tv.tv_nsec = %llu (hex: %llx)\n", (long long) tv->tv_nsec, (long long) tv->tv_nsec);
    return 0;
  }
}

static int test_clock_settime(struct timespec *tv)
{
  printf("Calling clock_settime():\n");
  printf("- tv.tv_sec = %llu (hex: %llx)\n", (long long) tv->tv_sec, (long long) tv->tv_sec);
  printf("- tv.tv_nsec = %llu (hex: %llx)\n", (long long) tv->tv_nsec, (long long) tv->tv_nsec);
  int result = clock_settime(CLOCK_REALTIME, tv);
  printf("clock_settime() returned %d\n", result);
  if (result)
  {
	  printf("- errno = %d\n", errno);
	  printf("- strerror(errno) =\"%s\"\n", strerror(errno));
	  return 1;
  }
  else
    return 0;
}

static void test_clock_gettime_settime(void)
{
  struct timespec tv;

  memset(&tv, 0x55, sizeof(tv));
  printf("\n");
  printf("**********************************************\n");
  printf("** Testing clock_gettime and clock_settime  **\n");
  printf("**********************************************\n");
  printf("\n");
  printf("Size of time_t: %u bytes\n", sizeof(time_t)); 
  printf("Size of struct timespec: %u bytes\n", sizeof(struct timespec)); 
  /* get current time, whatever it is */
  test_clock_gettime(&tv);
  /* test with current time just after Y2038 */
  tv.tv_nsec = 0x7FFFFFFF;
  tv.tv_nsec++;
  test_clock_settime(&tv);
  /* get current time; what is it now? */
  test_clock_gettime(&tv);
  printf("\n");
  printf("**********************************************\n");
  printf("** Tested clock_gettime and clock_settime  **\n");
  printf("**********************************************\n");
  printf("\n");
}

int main(int argc, char*argv[])
{ 
  int err = print_kernel_version();
  if (!err) err = print_glibc_version();
  
  test_clock_gettime_settime();

  while(1);

  return 0;
}


