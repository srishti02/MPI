
pure function interval_first_index(rank,nprocs,num_intervals)
  use, intrinsic :: ISO_Fortran_env
  integer(kind=INT64) :: interval_first_index
  integer, intent(in) :: rank, nprocs
  integer(kind=INT64), intent(in) :: num_intervals
  integer(kind=INT64) :: retval, min_chunk_width, left_over

  ! if the number of intervals is zero, then return 0...
  if (num_intervals .eq. 0) then
    retval = 0
  else
    ! determine the smallest chunk length...
    min_chunk_width = num_intervals / nprocs

    ! determine the remainder when only using the smallest chunk length...
    left_over = num_intervals - nprocs * min_chunk_width

    ! rank-th index is rank * min_chunk_width + the number of left_overs
    ! added to previous rank values (one per rank is added)... 
    retval = rank * min_chunk_width
    if (left_over .ne. 0) then
      if (rank .lt. left_over) then
        retval = retval + rank
      else
        retval = retval + left_over
      end if
    end if
  end if

  ! set return value...
  interval_first_index = retval
end function

program main
  use mpi_f08

  use, intrinsic :: ISO_Fortran_env
  implicit none

  integer(kind=INT64) :: interval_first_index ! external function
  integer(kind=INT32) :: error, rank, nprocs
  integer(kind=INT64) :: n, i, first_index, last_index
  real(kind=REAL64) :: overall_time_point = 0.0, time_point
  real(kind=REAL64) :: overall_estimate, sum, delta_x, neg_half_delta_x, midpoint

  call MPI_Init(error)
  call MPI_Comm_size(MPI_COMM_WORLD, nprocs, error)
  call MPI_Comm_rank(MPI_COMM_WORLD, rank, error)

  ! track the overall time...
  if (rank .eq. 0) then
    overall_time_point = MPI_Wtime()
  end if

  ! n is the number of intervals...
  n = 1
  do while (n <= 1000000000)
    ! record start time...
    time_point = 0.0d0
    if (rank .eq. 0) then
      time_point = MPI_Wtime()
    end if

    ! tell all processes the number of intervals we want...
    overall_estimate = 0.0d0
    call MPI_Bcast(n, 1, MPI_INTEGER, 0, MPI_COMM_WORLD, error)

    ! Example of non-MPI multithreaded Fortran code (using OpenMP)...
    sum = 0.0d0;
    delta_x = 1.0d0 / n
    neg_half_delta_x = -(delta_x/2.0d0)
    first_index = interval_first_index(rank, nprocs, n)
    last_index = interval_first_index(rank+1, nprocs, n)

    !$omp parallel do &
    !$omp& default(none) &
    !$omp& private(midpoint) &
    !$omp& shared(first_index,last_index,neg_half_delta_x,delta_x) &
    !$omp& reduction(+:sum)
    do i = first_index, last_index-1
      midpoint = neg_half_delta_x + i*delta_x
      sum = sum + 1.0d0 / (1.0d0 + midpoint*midpoint) * delta_x
    end do
    !$omp end parallel do

    ! perform one last set of sums with the result from each process...
    call MPI_Reduce(sum, overall_estimate, 1, MPI_DOUBLE, MPI_SUM, 0, &
      MPI_COMM_WORLD, error)

    ! estimate is off by a factor of 4 so adjust...
    overall_estimate = overall_estimate * 4.0d0

    ! output this for loop's results...
    if (rank .eq. 0) then
      time_point = MPI_Wtime() - time_point
      write(*,*) 'n: ',n,', pi: ',overall_estimate,', time: ',time_point,' seconds'
    end if

    n = n * 10
  end do

  if (rank == 0) then
    overall_time_point = MPI_Wtime() - overall_time_point
    write(*,*) 'overall_time: ',overall_time_point,' seconds'
  end if

  call MPI_Finalize(error)
end
