#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
  int rank, comm_size;
  double time_point = 0.0;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

  if (rank == 0)
  {
    time_point = MPI_Wtime();
    printf("helloworld: rank 0 process of %d processes.\n", comm_size);
  }

  MPI_Barrier(MPI_COMM_WORLD);
  printf("  ** Process %d outputs, \"Hello world!\"\n", rank);
  MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0)
  {
    time_point = MPI_Wtime() - time_point;
    printf("helloworld: elapsed time: %lf seconds.\n", time_point);
  }

  MPI_Finalize();
}
