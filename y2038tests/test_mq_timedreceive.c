#include <mqueue.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "tests.h"

static void test_mq_timedreceive_onqueue(mqd_t q)
{
  char msg[8] = "ABCD1234";
  int prio;

  test_begin("mq_timedreceive() with a time-out in the past before Y2038");
  struct timespec ts;
  int result = timespec_get(&ts, TIME_UTC);
  if (result != TIME_UTC)
    test_failure(1, "timespec_get returned %d instead of %d (TIME_UTC)", result, TIME_UTC);
  else
  {
    result = mq_timedreceive(q, msg, sizeof(msg), &prio, &ts);
    if (result != -1)
      test_failure(1, "mq_timedreceive returned %d", result);
    else if (errno != ETIMEDOUT)
      test_failure(1, "mq_timedreceive did not time out");
    else
      test_success();
  }

  test_begin("mq_timedreceive() with a time-out in the future before Y2038");
  result = timespec_get(&ts, TIME_UTC);
  if (result != TIME_UTC)
    test_failure(1, "timespec_get returned %d instead of %d (TIME_UTC)", result, TIME_UTC);
  else
  {
    ts.tv_sec++;
    result = mq_timedreceive(q, msg, sizeof(msg), &prio, &ts);
    if (result != -1)
      test_failure(1, "mq_timedreceive returned %d", result);
    else if (errno != ETIMEDOUT)
      test_failure(1, "mq_timedreceive did not time out");
    else
      test_success();
  }

  test_begin("Get current time");
  time_t t0 = time(NULL);
  if (t0==(time_t)-1)
  {
    test_failure(1, "time returned -1");
    return;
  }
  test_success();

  test_begin("Set current time at Y2038+1s");
  time_t t = 0x80000001;
  result = stime(&t);
  if (result)
  {
    test_failure(1, "stime returned %d", result);
    return;
  }
  test_success();

  test_begin("mq_timedreceive() with a time-out in the past after Y2038");
  result = timespec_get(&ts, TIME_UTC);
  if (result != TIME_UTC)
    test_failure(1, "timespec_get returned %d instead of %d (TIME_UTC)", result, TIME_UTC);
  else
  {
    result = mq_timedreceive(q, msg, sizeof(msg), &prio, &ts);
    if (result != -1)
      test_failure(1, "mq_timedreceive returned %d", result);
    else if (errno != ETIMEDOUT)
      test_failure(1, "mq_timedreceive did not time out");
    else
      test_success();
  }

  test_begin("mq_timedreceive() with a time-out in the future after Y2038");
  result = timespec_get(&ts, TIME_UTC);
  if (result != TIME_UTC)
    test_failure(1, "timespec_get returned %d instead of %d (TIME_UTC)", result, TIME_UTC);
  else
  {
    ts.tv_sec++;
    result = mq_timedreceive(q, msg, sizeof(msg), &prio, &ts);
    if (result != -1)
      test_failure(1, "mq_timedreceive returned %d", result);
    else if (errno != ETIMEDOUT)
      test_failure(1, "mq_timedreceive did not time out");
    else
      test_success();
  }

  test_begin("Restore current time");
  result = stime(&t0);
  if (result) test_failure(1, "stime returned %d", result); else test_success();
}

void test_mq_timedreceive(void)
{
  test_begin("Create the message queue");
  struct mq_attr mq_attr = { .mq_maxmsg = 1, .mq_msgsize = 8 };
  mqd_t q = mq_open("/y2038", O_RDWR | O_CREAT, 0x777, &mq_attr);
  if (q == (mqd_t) -1)
  {
    test_failure(1, "mq_open returned -1");
    return;
  }
  test_success();

  test_mq_timedreceive_onqueue(q);

  test_begin("Close the message queue");
  int cq = mq_close(q);
  if (cq)
    test_failure(1, "mq_close returned %d", cq);
  else
    test_success();
}
