#include <stdio.h>
#include "id_kernel.h"
#include "id_glibc.h"

#include "tests.h"

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
#include "test_futimens.h"
#include "test_utimensat.h"
#include "test_timer_gettime_settime.h"
#include "test_timerfd_gettime_settime.h"
#include "test_stat.h"
#include "test_fstat.h"
#include "test_lstat.h"
#include "test_fstatat.h"

int main(int argc, char*argv[])
{ 
  int err = print_kernel_version();
  if (!err) err = print_glibc_version();
  
  if (err) return 1;

  printf("\n");

  tests_init();

  test_clock_gettime_settime();
  test_difftime();
  test_ctime();
  test_gmtime();
  test_gmtime_r();
  test_mktime();
  test_ctime_r();
  test_timegm();
  test_clock_nanosleep();
  test_timespec_get();
  test_futimens();
  test_utimensat();
  test_timer_gettime_settime();
  test_timerfd_gettime_settime();
  test_stat();
  test_fstat();
  test_lstat();
  test_fstatat();

  tests_report();

  return 0;
}


