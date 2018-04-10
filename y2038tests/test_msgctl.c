#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "tests.h"

static void test_msgctl_onqueue(int q)
{
  typedef struct
  {
    long mtype;
    char mtext[1];
  } msg_t;

  msg_t msgs = { .mtype = 42, .mtext = { 'X' } };
  msg_t msgr = { .mtype = -1, .mtext = { 'Z' } };

  test_begin("call msgctl() with a never-used queue");
  struct msqid_ds stat;
  int result = msgctl(q, IPC_STAT, &stat);
  if (result)
    test_failure(1, "msgctl returned %d", result);
  else if (stat.msg_stime)
    test_failure(0, "msgctl: stat.msg_stime = %lld, expected 0", (long long) stat.msg_stime);
  else if (stat.msg_rtime)
    test_failure(0, "msgctl: stat.msg_rtime = %lld, expected 0", (long long) stat.msg_rtime);
  else
    test_success();

  test_begin("call msgctl() after a single queue write");
  result = msgsnd(q, &msgs, sizeof(msgs.mtext), IPC_NOWAIT);
  if (result)
    test_failure(1, "msgsnd returned %d", result);
  else
  {
    result = msgctl(q, IPC_STAT, &stat);
    if (result)
      test_failure(1, "msgctl returned %d", result);
    else if (stat.msg_stime == 0)
      test_failure(0, "msgctl: stat.msg_stime = 0, expected nonzero");
    else if (stat.msg_rtime)
      test_failure(0, "msgctl: stat.msg_rtime = %lld, expected 0", (long long) stat.msg_rtime);
    else
      test_success();
  }

  test_begin("call msgctl() after a queue write and read");
  result = msgsnd(q, &msgs, sizeof(msgs.mtext), IPC_NOWAIT);
  if (result)
    test_failure(1, "msgsnd returned %d", result);
  else
  {
    result = msgrcv(q, &msgr, sizeof(msgs.mtext), 0, IPC_NOWAIT);
    if (result < 0)
      test_failure(1, "msgrcv returned %d", result);
    else if (result != sizeof(msgs.mtext))
      test_failure(0, "msgrcv returned %d, expected %d", result, sizeof(msgs.mtext));
    else
    {
      result = msgctl(q, IPC_STAT, &stat);
      if (result)
        test_failure(1, "msgctl returned %d", result);
      else if (stat.msg_stime == 0)
        test_failure(0, "msgctl: stat.msg_stime = 0, expected nonzero");
      else if (stat.msg_rtime == 0)
        test_failure(0, "msgctl: stat.msg_rtime = 0, expected nonzero");
      else if (msgr.mtype != 42)
        test_failure(0, "msgctl: msgr.mtype = %ld, expected 42", msgr.mtype);
      else if (msgr.mtext[0] != 'X')
        test_failure(0, "msgctl: msgr.mttext = (char)%d, expected 'X'", msgr.mtext[0]);
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
    test_failure(1, "msgget returned -1");
    return;
  }
  test_success();

  test_msgctl_onqueue(q);

  test_begin("Close the msgctl test SysV message queue");
  int r = msgctl(q, IPC_RMID, NULL);
  if (r)
    test_failure(1, "msgctl returned %d", r);
  else
    test_success();
}
