//===========================================================================

#include <stdio.h>
#include <mpi.h>

//===========================================================================

typedef unsigned long long interval_type;

interval_type interval_first_index(
  interval_type const rank,
  interval_type const nprocs,
  interval_type const num_intervals
) 
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
      fprintf(stderr, "ERROR: MPI unable to establish required threading "
        "environment. Aborting.\n");
      MPI_Abort(MPI_COMM_WORLD, 1);
      return 1;
    }
  }

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
  for (interval_type n=1; n <= 10000000000ULL; n *= 10)
  {
    // record start time...
    double time_point = 0.0;
    if (rank == 0)
      time_point = MPI_Wtime();

    // tell all processes the number of intervals we want...
    MPI_Bcast(&n, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

    //=======================================================================
    // Example of non-MPI multithreaded C code (using OpenMP)
    double sum = 0.0; // Will hold OpenMP result...
    {
      double const delta_x = 1.0 / n;
      double const half_delta_x = delta_x/2.0;
      double const neg_half_delta_x = -half_delta_x;

      interval_type first_index = interval_first_index(rank, nprocs, n);
      interval_type last_index = interval_first_index(rank+1, nprocs, n);
        
      #pragma omp parallel for \
        default(none) \
        shared(first_index,last_index,neg_half_delta_x,delta_x) \
        reduction(+:sum)
      for (interval_type i=first_index; i != last_index; ++i)
      {
        double const midpoint = neg_half_delta_x + i*delta_x;
        double const interval_estimate = 1.0 / (1.0 + midpoint*midpoint) * delta_x;
        sum += interval_estimate;
      }
    }

    // perform one last set of sums with the result from each process...
    double overall_estimate = 0.0;
    MPI_Reduce(&sum, &overall_estimate, 1, MPI_DOUBLE, MPI_SUM, 0, 
      MPI_COMM_WORLD);

    // estimate if off by a factor of 4 so adjust...
    overall_estimate *= 4.0;

    // output this for loop's results...
    if (rank == 0)
    {
      time_point = MPI_Wtime() - time_point;
      printf("n: %llu, pi: %.16lg (hex: %la), time: %lg seconds\n", 
        n, overall_estimate, overall_estimate, time_point);
    }
  }

  if (rank == 0)
  {
    overall_time_point = MPI_Wtime() - overall_time_point;
    printf("overall_time: %lg seconds\n", overall_time_point);
  }

  MPI_Finalize();
}
