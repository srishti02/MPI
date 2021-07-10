//===========================================================================

#include <limits>       // for std::numeric_limits
#include <numeric>      // for std::transform_reduce
#include <functional>   // for std::plus
#include <execution>    // for std::execution
#include <iostream>     // for std::cout, std::cerr
#include <iomanip>      // for std::precision
#include <utility>      // for std::forward

// Undocumented OpenMPI hack to disable GCC 9+ warnings...
// e.g., see https://github.com/open-mpi/ompi/issues/5157
#define OMPI_SKIP_MPICXX 1
#include <mpi.h>

#include "paralg_tbb_control.hpp"
#include "iota_iterator.hpp"
#include "numerically_integrate.hpp"

//===========================================================================

//
// Demonstrate using something other than an int to iterate through 
// intervals to show MPI_UNSIGNED_LONG_LONG needs to be used instead
// of MPI_INT. Such is defined as a type alias to make it clearer in code.
//
using interval_type = unsigned long long;

//
// interval_first_index(rank,nprocs,num_intervals)
//
// This function returns the starting index of the rank-th interval
// given num_intervals divided evenly to nprocs processes. If nprocs
// does not evenly divide num_intervals then num_intervals % nprocs
// values are evenly spread over the first M intervals until there are
// no more values to spread. Should num_intervals % nprocs result in
// zero, then there will be exactly one interval.
//
// Suppose num_intervals == 20 and nprocs == 3, then the intervals
// would be: [0,6+1), [7,13+1), [14,20) since 20 / 3 == 6 remainder 2.
//
constexpr interval_type interval_first_index(
  interval_type const& rank, 
  interval_type const& nprocs, 
  interval_type const& num_intervals
) noexcept
{
  // if the number of intervals is zero, then return 0...
  if (!num_intervals)
    return 0;

  // determine the smallest chunk length...
  interval_type const min_chunk_width = num_intervals / nprocs;

  // determine the remainder when only using the smallest chunk length...
  interval_type const left_over = num_intervals - nprocs * min_chunk_width;

  // rank-th index is rank * min_chunk_width + the number of left_overs 
  // added to previous rank values (one per rank is added)...
  interval_type retval = rank * min_chunk_width;
  if (left_over)
  {
    if (rank < left_over)
      retval += rank;
    else
      retval += left_over;
  }
  return retval;
}

//===========================================================================

int main(int argc, char *argv[])
{
  using sharcnet_summer_school_2021::iota_iterator;
  using sharcnet_summer_school_2021::numerically_integrate;
  using sharcnet_summer_school_2021::use_env_num_of_threads_or_provided;

  //=========================================================================
  // disable syncing with C stdio calls...
  //=========================================================================
  std::ios::sync_with_stdio(false);

  //=========================================================================
  // Tell MPI A hyrbid process/threaded environment is required.
  //   * MPI_THREAD_FUNNELED means only the main process thread 
  //     (for each rank) can call MPI functions.
  //=========================================================================
  {
    int mpi_init_provided = 0;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &mpi_init_provided);
    if (mpi_init_provided < MPI_THREAD_MULTIPLE)
    {
      std::cerr << "ERROR: MPI unable to establish required threading "
        "environment. Aborting.\n";
      MPI_Abort(MPI_COMM_WORLD, 1);
      return 1;
    }
  }

  //=========================================================================
  // If TBB is present, ask underlying TBB library to use the number of 
  // threads in SLURM_CPUS_PER_TASK or OMP_NUM_THREADS or 1 thread.
  //   NOTE: If TBB does not exist then the number of threads will be 
  //         std::hardware_concurrency();
  // NOTE: This is needed since C++17 and C++20 don't define anything to
  //       control parallel execution policies (e.g., number of threads).
  //=========================================================================
  auto&& dont_delete = use_env_num_of_threads_or_provided(1);

  //=========================================================================
  // obtain the rank and number of processes...
  //=========================================================================
  int rank, nprocs;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

  //=========================================================================
  // track the overall time...
  //=========================================================================
  double overall_time_point = 0.0;
  if (rank == 0)
    overall_time_point = MPI_Wtime();

  //=========================================================================
  // n is the number of intervals so start at 1 and increase ten-fold each
  // iteration...
  //=========================================================================
  for (interval_type n=1; n <= 10'000'000'000ULL; n *= 10)
  {
    // record start time...
    double time_point = 0.0;
    if (rank == 0)
      time_point = MPI_Wtime();

    // tell all processes the number of intervals we want...
    MPI_Bcast(&n, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

    //=======================================================================
    // Example of non-MPI multithreaded C++ code
    //
    // Each thread will estimate pi over its sub-interval (see the last two
    // arguments):
    //   * This code is run in parallel using threads on the same computer
    //     using C++17's parallel algorithm std::transform_reduce().
    //   * [nth_chunk_first_index,nth_chunk_last_index) is the range
    //     of the nth interval in terms of n (which is an integer).
    double const sum = numerically_integrate(
      std::execution::par_unseq,
      n, 0.0, 1.0,
      [](auto const& x) constexpr { return 1.0 / (1.0 + x*x); },
      iota_iterator<interval_type>(interval_first_index(rank, nprocs, n)),
      iota_iterator<interval_type>(interval_first_index(rank+1, nprocs, n))
    );
    //=======================================================================

    // perform one last set of sums with the result from each process...
    double overall_estimate = 0.0;
    MPI_Reduce(&sum, &overall_estimate, 1, MPI_DOUBLE, MPI_SUM, 0, 
      MPI_COMM_WORLD);
    overall_estimate *= 4.0; // sum is pi/4, adjust to equal pi

    // output this for loop's results...
    if (rank == 0)
    {
      time_point = MPI_Wtime() - time_point;
      std::cout << "n: " << n << ", pi: ";
      auto const prev_precision = std::cout.precision();
      std::cout
        << std::setprecision(
          std::numeric_limits<decltype(overall_estimate)>::max_digits10
        ) << overall_estimate << std::setprecision(prev_precision)
        << " (hex: " << std::hexfloat << overall_estimate
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
