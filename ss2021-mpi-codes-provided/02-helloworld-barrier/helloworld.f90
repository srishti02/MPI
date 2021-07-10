program main
  use mpi_f08

  use, intrinsic :: ISO_Fortran_env
  implicit none

  integer(kind=INT32) error
  integer(kind=INT32) rank
  integer(kind=INT32) p
  real(kind=REAL64) time_point

  call MPI_Init(error)
  call MPI_Comm_size(MPI_COMM_WORLD, p, error)
  call MPI_Comm_rank(MPI_COMM_WORLD, rank, error)

  if (rank == 0) then
    time_point = MPI_Wtime()

    write(*, '(a,i8,a)') 'helloworld: rank 0 process of ', p, ' processes.'
  end if

  call MPI_Barrier(MPI_COMM_WORLD, error)
  write(*, '(a,i8,a)') '  ** Process ', rank, ' outputs, "Hello world!"'
  call MPI_Barrier(MPI_COMM_WORLD, error)

  if (rank == 0) then
    time_point = MPI_Wtime() - time_point
    write ( *, '(a,g14.6,a)' ) &
      'helloworld: elapsed time: ', time_point, ' seconds.'
  end if

  call MPI_Finalize(error)
end
