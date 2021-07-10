#include <iostream>

// Undocumented OpenMPI hack to disable GCC 9+ warnings...
// e.g., see https://github.com/open-mpi/ompi/issues/5157
#define OMPI_SKIP_MPICXX 1
#include <mpi.h>

int main(int argc, char *argv[])
{
  // Untie IOStream from C's stdio...
  std::ios::sync_with_stdio(false);

  MPI_Init(&argc, &argv);

  int rank; MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int comm_size; MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

  if (rank == 0 && comm_size != 2)
  {
    std::cerr << "ERROR: The number of processes is not 2! Aborting.\n";
    MPI_Abort(MPI_COMM_WORLD, 1);
    return 1;
  }

  if (rank == 0)
  {
    // process rank 0 will send the number 3.14 to process rank 1...
    double const pi = 3.14;
    if (MPI_Send(&pi, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD) == MPI_SUCCESS)
      std::cout << "Rank " << rank << " sent value " << pi << " to rank 1.\n";
    else
      std::cout << "Rank " << rank << " did not sucessfully send the value " 
        << pi << " to rank 1.\n";
  }
  else
  {
    // process rank 1 will receive the number 3.14 from process rank 0...
    double value = 0.0;
    MPI_Status status;
    if (MPI_Recv(&value, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status) == MPI_SUCCESS)
      std::cout << "Rank " << rank << " received value " << value << ".\n";
    else
      std::cout << "Rank " << rank << " did not successfully receive a value!\n";
  }

  MPI_Finalize();
}
