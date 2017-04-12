#include <stdio.h>
#include "id_kernel.h"
#include "id_glibc.h"
#include "test_clock_gettime_settime.h"
#include "test_ctime.h"
#include "test_difftime.h"
#include "test_gmtime.h"
#include "test_gmtime_r.h"
#include "test_mktime.h"
#include "test_ctime_r.h"
#include "test_timegm.h"
#include "test_clock_nanosleep.h"
#include "test_timespec_get.h"

int main(int argc, char*argv[])
{ 
  int tests_run = 0;
  int tests_fail = 0;

  int err = print_kernel_version();
  if (!err) err = print_glibc_version();
  
  if (err) return 1;

  printf("------------------------------------------\n");
  test_clock_gettime_settime(&tests_run, &tests_fail);
  test_difftime(&tests_run, &tests_fail);
  test_ctime(&tests_run, &tests_fail);
  test_gmtime(&tests_run, &tests_fail);
  test_gmtime_r(&tests_run, &tests_fail);
  test_mktime(&tests_run, &tests_fail);
  test_ctime_r(&tests_run, &tests_fail);
  test_timegm(&tests_run, &tests_fail);
  test_clock_nanosleep(&tests_run, &tests_fail);
  test_timespec_get(&tests_run, &tests_fail);
  printf("------------------------------------------\n");

  if (tests_run == 0)
  {
    printf("*** NO TEST RUN\n");
    return 1;
  }
  if (tests_fail > 0)
  {
    printf("*** %d OF %d TESTS FAILED\n", tests_fail, tests_run);
    return 2;
  }
  printf("*** ALL OF THE %d TESTS SUCCEEDED\n", tests_run);
  return 0;
}


