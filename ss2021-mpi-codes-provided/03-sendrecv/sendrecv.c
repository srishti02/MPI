#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
  int rank, comm_size;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

  if (rank == 0 && comm_size != 2)
  {
    fprintf(stderr, "ERROR: The number of processes is not 2! Aborting.\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
    return 1;
  }

  if (rank == 0)
  {
    // process rank 0 will send the number 3.14 to process rank 1...
    double const pi = 3.14;
    if (MPI_Send(&pi, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD) == MPI_SUCCESS)
      printf("Rank %d sent value %lf to rank 1.\n", rank, pi);
    else
      printf("Rank %d did not sucessfully send the value %lf to rank 1.\n", rank, pi);
  }
  else
  {
    // process rank 1 will receive the number 3.14 from process rank 0...
    double value = 0.0;
    MPI_Status status;
    if (MPI_Recv(&value, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status) == MPI_SUCCESS)
      printf("Rank %d received value %lf.\n", rank, value);
    else
      printf("Rank %d did not successfully receive a value!\n", rank);
  }

  MPI_Finalize();
}
