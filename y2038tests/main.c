#include <stdio.h>
#include <time.h>
#include <stdarg.h>
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
#include "test_time_stime.h"
#include "test_gettimeofday_settimeofday.h"
#include "test_mq_timedreceive.h"
#include "test_mq_timedsend.h"
#include "test_msgctl.h"
#include "test_sched_rr_get_interval.h"
#include "test_nanosleep.h"
#include "test_adjtime.h"
#include "test_utime.h"
#include "test_set_getitimer.h"

# if defined(_TIME_BITS) && _TIME_BITS==64
 #define _TIME_T_SIZE 8
#else
 #define _TIME_T_SIZE 4
#endif

int main(int argc __attribute__((unused)), char*argv[] __attribute__((unused)))
{ 
  int time_t_size = sizeof(time_t);
  if (time_t_size != _TIME_T_SIZE)
  {
      printf("sizeof(time_t) = %d (expected %d)\n", time_t_size, _TIME_T_SIZE) ;
      return 1;
  }
  int err = print_kernel_version();
  if (!err) err = print_glibc_version();
  
  if (err) return 1;

  printf("\n");

  tests_init();

  test_run(test_clock_gettime_settime);
  test_run(test_difftime);
  test_run(test_ctime);
  test_run(test_gmtime);
  test_run(test_gmtime_r);
  test_run(test_mktime);
  test_run(test_ctime_r);
  test_run(test_timegm);
  test_run(test_clock_nanosleep);
  test_run(test_timespec_get);
  test_run(test_futimens);
  test_run(test_utimensat);
  test_run(test_timer_gettime_settime);
  test_run(test_timerfd_gettime_settime);
  test_run(test_stat);
  test_run(test_fstat);
  test_run(test_lstat);
  test_run(test_fstatat);
  test_run(test_time_stime);
  test_run(test_gettimeofday_settimeofday);
  test_run(test_mq_timedreceive);
  test_run(test_mq_timedsend);
  test_run(test_msgctl);
  test_run(test_sched_rr_get_interval);
  test_run(test_nanosleep);
  test_run(test_adjtime);
  test_run(test_utime);
  test_run(test_set_getitimer);

  tests_report();

  return 0;
}


