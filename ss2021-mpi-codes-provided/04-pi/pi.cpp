#include <limits>
#include <iostream>
#include <iomanip>

// Undocumented OpenMPI hack to disable GCC 9+ warnings...
// e.g., see https://github.com/open-mpi/ompi/issues/5157
#define OMPI_SKIP_MPICXX 1
#include <mpi.h>

int main(int argc, char *argv[])
{
  // Untie IOStreams from C stdio...
  std::ios::sync_with_stdio(false);

  MPI_Init(&argc, &argv);

  int rank; MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int num_processes; MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  
  // track the overall time...
  double overall_time_point = 0.0;
  if (rank == 0)
    overall_time_point = MPI_Wtime();
  
  // n is the number of intervals...
  for (unsigned long long n=1; n <= 10000000000ULL; n *= 10)
  {
    // record start time...
    double time_point = 0.0;
    if (rank == 0)
      time_point = MPI_Wtime();

    // tell all processes the number of intervals we want...
    double overall_pi_estimate = 0.0;
    MPI_Bcast(
      &n,                     // the number of intervals
      1,                      // first argument is a scalar (1 element)
      MPI_UNSIGNED_LONG_LONG, // first argument is an unsigned long long
      0,                      // tag value
      MPI_COMM_WORLD          // communication group
    );

    // Each process will estimate pi over number of intervals...
    //   * This code is sequential within each process.
    //   * Each process has its own unique rank id value.
    //   * The rank == 0 process also performs work: it does not
    //     sit waiting for results.
    //   * Since there are num_processes processes the amount of
    //     work done by each process is effectively n / num_processes
    //     which results in speed-up relative to only using one process.
    double const interval_width = 1.0 / n;
    double sum = 0.0;
    for (unsigned long long i = rank+1; i <= n; i += num_processes)
    {
      double x = interval_width * (i-0.5); // interval midpoint
      sum += 4.0 / (1.0 + x*x);
    }

    // perform one last set of sums with the result from each process...
    double const a_pi_estimate = interval_width * sum;
    MPI_Reduce(
      &a_pi_estimate,         // pi estimate from this process
      &overall_pi_estimate,   // final MPI_SUM result goes here
      1,                      // i.e., first argument is 1 element
      MPI_DOUBLE,             // i.e., first argument points to type double
      MPI_SUM,                // i.e., reduction operation is sum
      0,                      // tag value
      MPI_COMM_WORLD          // communication group
    );

    // output this for loop's results...
    if (rank == 0)
    {
      time_point = MPI_Wtime() - time_point;
      std::cout 
        << "n: " << n 
        << ", pi: "
      ; 
      auto const prev_precision = std::cout.precision();
      std::cout 
        << std::setprecision(
          std::numeric_limits<decltype(overall_pi_estimate)>::max_digits10
        ) << overall_pi_estimate << std::setprecision(prev_precision)
        << " (hex: " << std::hexfloat << overall_pi_estimate 
        << "), time: " << std::defaultfloat << time_point 
        << " seconds\n"
      ;
    }
  }

  if (rank == 0)
  {
    overall_time_point = MPI_Wtime() - overall_time_point;
    std::cout << "overall_time: " << overall_time_point << " seconds\n";
  }

  MPI_Finalize();
}
