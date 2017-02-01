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

#include "id_kernel.h"
#include "id_glibc.h"

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


