#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <mpi.h>

#include "rma_mutex.h"

/* lock wait timeout (seconds) before assuming deadlock */
#define DEADLOCK_TIMEOUT 5

#define with_timeout(m, t, f) do { \
  set_timer(m, t); \
  f \
  alarm(0); \
} while (0);

struct {
  int len;
  char *msg;
} timeout_msg;

void timeout_handler(int sig)
{
  write(STDERR_FILENO, timeout_msg.msg, timeout_msg.len);
  exit(EXIT_FAILURE);
}

void set_timer(const char *msg, int t)
{
  timeout_msg.len = strlen(msg);
  timeout_msg.msg = malloc(timeout_msg.len + 1);
  assert(timeout_msg.msg != NULL);
  strcpy(timeout_msg.msg, msg);
  signal(SIGALRM, timeout_handler);
  alarm(t);
}

int main(int argc, char **argv)
{
  rma_mutex_t m;

  assert(MPI_Init(&argc, &argv) == MPI_SUCCESS);

  rma_mutex_init(&m, MPI_COMM_WORLD, 0);

  /* start deadlock timer */
  with_timeout("error: timeout in lock acquisition; assuming deadlock\n",
    DEADLOCK_TIMEOUT, {
      /* drop some ranks, in an effort to detect incorrect lock assignee
       * indexing (which can be missed if all ranks contending for lock) */
      if (m.rank % 3 != 0) {
        rma_mutex_lock(&m);

        printf("%3.3i has lock\n", m.rank); fflush(stdout);

        printf("%3.3i about to release lock\n", m.rank); fflush(stdout);

        rma_mutex_unlock(&m);
      }
    });

  rma_mutex_free(&m);
  
  assert(MPI_Finalize() == MPI_SUCCESS);

  return 0;
}
