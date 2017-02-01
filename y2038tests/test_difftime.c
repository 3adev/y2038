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

#if defined _TIME_BITS && _TIME_BITS == 64
#define FMT "%ll"
#else
#define FMT "%l"
#endif

static int test_difftime_call(time64_t time1, time64_t time0)
{
  long double result = difftime64(time1, time0);
  printf("diftime(" FMT "d, " FMT "d) returned %Lg\n", time1, time0, result);
}

static void test_difftime(void)
{
  printf("\n");
  printf("**********************************************\n");
  printf("** Testing clock_difftime                   **\n");
  printf("**********************************************\n");
  printf("\n");
  printf("Size of time64_t: %u bytes\n", sizeof(time64_t)); 
  printf("----------------------------------------------\n");
  printf("Testing difftime() for time1 and time0 before Y2038:\n");
  test_difftime_call(3600ull, 7200ull);
  test_difftime_call(7200ull, 3600ull);
  printf("----------------------------------------------\n");
  printf("Testing difftime() for time1 and time0 before Y2038:\n");
  test_difftime_call(0x90000000ull+3600ull, 0x90000000ull+7200ull);
  test_difftime_call(0x90000000ull+7200ull, 0x90000000ull+3600ull);
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
  
  test_difftime();

  return 0;
}


