#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "tests.h"

static void test_msgctl_onqueue(int q)
{
  struct
  {
    long mtype;
    char mtext[1];
  } msgs = { 42, 'X' }, msgr = { -1, 'Z' };

  test_begin("call msgctl() with a never-used queue");
  struct msqid_ds stat;
  int result = msgctl(q, IPC_STAT, &stat);
  if (result)
    test_failure();
  else if (stat.msg_stime)
    test_failure();
  else if (stat.msg_rtime)
    test_failure();
  else
    test_success();

  test_begin("call msgctl() after a single queue write");
  result = msgsnd(q, &msgs, sizeof(msgs.mtext), IPC_NOWAIT);
  if (result)
    test_failure();
  else
  {
    result = msgctl(q, IPC_STAT, &stat);
    if (result)
      test_failure();
    else if (stat.msg_stime == 0)
      test_failure();
    else if (stat.msg_rtime)
      test_failure();
    else
      test_success();
  }

  test_begin("call msgctl() after a queue write and read");
  result = msgsnd(q, &msgs, sizeof(msgs.mtext), IPC_NOWAIT);
  if (result)
    fprintf(stderr, "Line %d", __LINE__), test_failure();
  else
  {
    result = msgrcv(q, &msgr, sizeof(msgs.mtext), 0, IPC_NOWAIT);
    if (result < 0)
      fprintf(stderr, "Line %d err %d %s\n", __LINE__, errno, strerror(errno)), test_failure();
    else if (result < sizeof(msgs.mtext))
      fprintf(stderr, "Line %d err %d %s\n", __LINE__, errno, strerror(errno)), test_failure();
    else
    {
      result = msgctl(q, IPC_STAT, &stat);
      if (result)
        fprintf(stderr, "Line %d err %d %s\n", __LINE__, errno, strerror(errno)), test_failure();
      else if (stat.msg_stime == 0)
        fprintf(stderr, "Line %d err %d %s\n", __LINE__, errno, strerror(errno)), test_failure();
      else if (stat.msg_rtime == 0)
        fprintf(stderr, "Line %d err %d %s\n", __LINE__, errno, strerror(errno)), test_failure();
      else if (msgr.mtype != 42)
        fprintf(stderr, "Line %d err %d %s\n", __LINE__, errno, strerror(errno)), test_failure();
      else if (msgr.mtext[0] != 'X')
        fprintf(stderr, "Line %d err %d %s\n", __LINE__, errno, strerror(errno)), test_failure();
      else
        test_success();
    }
  }
}

void test_msgctl(void)
{
  test_begin("Create the SysV message queue for msgctl tests");
  int q = msgget(IPC_PRIVATE, 0x777);
  if (q == -1)
  {
    test_failure();
    return;
  }
  test_success();

  test_msgctl_onqueue(q);

  test_begin("Close the msgctl test SysV message queue");
  if (msgctl(q, IPC_RMID, NULL))
    test_failure();
  else
    test_success();
}
