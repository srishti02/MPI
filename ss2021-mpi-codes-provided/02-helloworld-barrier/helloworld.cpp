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

  double time_point = 0.0;
  if (rank == 0)
  {
    time_point = MPI_Wtime();
    std::cout << "helloworld: rank 0 process of " << comm_size << " processes.\n" << std::endl;
  }

  MPI_Barrier(MPI_COMM_WORLD);
  std::cout << "  ** Process " << rank << " outputs, \"Hello world!\"\n";
  MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0)
  {
    time_point = MPI_Wtime() - time_point;
    std::cout << "helloworld: elapsed time: " << time_point << " seconds.\n";
  }

  MPI_Finalize();
}
