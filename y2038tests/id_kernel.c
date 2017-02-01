#include "id_kernel.h"
#include <stdio.h>
#include <sys/utsname.h>
#include <errno.h>
#include <string.h>

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
