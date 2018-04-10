#include "id_kernel.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int print_glibc_version(void)
{
  char *confbuf;
  unsigned int conflen;

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

