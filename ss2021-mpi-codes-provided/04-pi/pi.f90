program main
  use mpi_f08

  use, intrinsic :: ISO_Fortran_env
  implicit none

  integer(kind=INT32) :: error, rank, nprocs
  integer(kind=INT64) :: n, i
  real(kind=REAL64) :: overall_time_point = 0.0, time_point
  real(kind=REAL64) :: global_pi, a_pi
  real(kind=REAL64) :: interval_width, sum, x

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
    global_pi = 0.0d0
    call MPI_Bcast( &
      n, &                      ! the number of intervals
      1, &                      ! first argument is a scalar (1 element)
      MPI_INTEGER, &            ! first argument is an unsigned long long
      0, &                      ! tag value
      MPI_COMM_WORLD, &         ! communication group
      error &
    )

    ! Each process will estimate pi over number of intervals...
    !   * This code is sequential within each process.
    !   * Each process has its own unique rank id value.
    !   * The rank == 0 process also performs work: it does not
    !     sit waiting for results.
    !   * Since there are nprocs processes the amount of
    !     work done by each process is effectively n / nprocs
    !     which results in speed-up relative to only using one process.
    interval_width = 1.0d0 / n
    sum = 0.0d0
    do i = rank+1, n, nprocs
      x = interval_width * (dble(i)-0.5d0)  ! interval midpoint
      sum = sum + 4.0d0 / (1.0d0 + x*x)
    end do

    ! perform one last set of sums with the result from each process...
    a_pi = interval_width * sum
    call MPI_Reduce( &
      a_pi, &                 ! pi estimate from this process
      global_pi, &            ! final MPI_SUM result goes here
      1, &                    ! i.e., first argument is 1 element
      MPI_DOUBLE, &           ! i.e., first argument points to type double
      MPI_SUM, &              ! i.e., reduction operation is sum
      0, &                    ! tag value
      MPI_COMM_WORLD, &       ! communication group
      error &
    )

    ! output this for loop's results...
    if (rank .eq. 0) then
      time_point = MPI_Wtime() - time_point
      write(*,*) 'n: ',n,', pi: ',global_pi,', time: ',time_point,' seconds'
    end if

    n = n * 10
  end do

  if (rank == 0) then
    overall_time_point = MPI_Wtime() - overall_time_point
    write(*,*) 'overall_time: ',overall_time_point,' seconds'
  end if

  call MPI_Finalize(error)
end
