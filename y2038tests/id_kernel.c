#include "id_kernel.h"
#include <stdio.h>
#include <sys/utsname.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int print_kernel_version(void)
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

int check_kernel_timekeeping(void)
{
  int result;
  // Set time to half a second before Y2038 or fail
  struct timespec tv = { 0x7FFFFFFF, 500000000 };
  result = clock_settime(CLOCK_REALTIME, &tv);
  if (!result)
  {
    // Wait a second
    usleep(1000000);
    // Read back time or fail
    memset(&tv, 0, sizeof(tv));
    result = clock_gettime(CLOCK_REALTIME, &tv);
    if (!result)
    {
      // Fail iff time in seconds now negative.
      result = (tv.tv_sec < 0) ? 1: 0;
    }
  }
  if (result < 0)
  {
    printf("\n");
    printf("**********************************************\n");
    printf("** Kernel timekeeping could not be verified **\n");
    printf("**********************************************\n");
  }
  else if (result > 0)
  {
    printf("\n");
    printf("**********************************************\n");
    printf("** Kernel timekeeping not Y2038-proof!      **\n");
    printf("**********************************************\n");
  }
  return result;
}
