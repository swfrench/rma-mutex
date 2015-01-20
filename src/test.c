#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <mpi.h>

#include "rma_mutex.h"

int main(int argc, char **argv)
{
  rma_mutex_t m;

  assert(MPI_Init(&argc, &argv) == MPI_SUCCESS);

  rma_mutex_init(&m, MPI_COMM_WORLD, 0);

  rma_mutex_lock(&m);

  printf("%3.3i has lock\n", m.rank); fflush(stdout);

  printf("%3.3i about to release lock\n", m.rank); fflush(stdout);

  rma_mutex_unlock(&m);

  rma_mutex_free(&m);
  
  assert(MPI_Finalize() == MPI_SUCCESS);

  return 0;
}
