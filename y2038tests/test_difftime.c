#include <stdio.h>
#include <sys/utsname.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <inttypes.h>
#include <asm/unistd.h>

#include "id_kernel.h"
#include "id_glibc.h"

#if defined _TIME_BITS && _TIME_BITS == 64
#define FMTD "%" PRId64
#else
#define FMTD "%" PRId32
#endif

static int test_difftime_call(time64_t time1, time64_t time0)
{
  long double result = difftime64(time1, time0);
  printf("diftime(" FMTD ", " FMTD ") returned %Lg\n", time1, time0, result);
}

static void test_difftime(void)
{
  printf("\n");
  printf("**********************************************\n");
  printf("** Testing difftime                         **\n");
  printf("**********************************************\n");
  printf("\n");
  printf("Size of time64_t: %zu bytes\n", sizeof(time64_t)); 
  printf("----------------------------------------------\n");
  printf("Testing difftime() for time1 and time0 before Y2038:\n");
  test_difftime_call(3600ll, 7200ll);
  test_difftime_call(7200ll, 3600ll);
  printf("----------------------------------------------\n");
  printf("Testing difftime() for time1 and time0 after Y2038:\n");
  test_difftime_call(0x90000000dll+3600ll, 0x90000000dll+7200ll);
  test_difftime_call(0x90000000dll+7200ll, 0x90000000dll+3600ll);
  printf("\n");
  printf("**********************************************\n");
  printf("** Tested difftime                          **\n");
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


