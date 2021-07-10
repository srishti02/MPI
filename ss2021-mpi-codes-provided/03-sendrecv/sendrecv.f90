program main
  use mpi_f08

  use, intrinsic :: ISO_Fortran_env
  implicit none

  type(MPI_Status) status
  integer(kind=INT32) error
  integer(kind=INT32) rank
  integer(kind=INT32) p
  real(kind=REAL64), parameter :: pi = 3.14
  real(kind=REAL64) :: val = 0.0

  call MPI_Init(error)
  call MPI_Comm_size(MPI_COMM_WORLD, p, error)
  call MPI_Comm_rank(MPI_COMM_WORLD, rank, error)

  if ((rank .eq. 0) .and. (p .ne. 2)) then
    write(*,*) 'ERROR: The number of processes is not 2! Aborting.'
    call MPI_Abort(MPI_COMM_WORLD, 1, error)
    stop 1
  end if

  if (rank .eq. 0) then
    ! process rank 0 will send the number 3.14 to process rank 1...
    call MPI_Send(pi, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, error)
    if (error .eq. MPI_SUCCESS) then
      write(*,*) 'Rank ',rank,' sent value ',pi,' to rank 1.'
    else
      write(*,*) 'Rank ',rank,' did not sucessfully send the value ',pi,' to rank 1.'
    end if
  else
    ! process rank 1 will receive the number 3.14 from process rank 0...
    call MPI_Recv(val, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, status, error)
    if (error .eq. MPI_SUCCESS) then
      write(*,*) 'Rank ',rank,' received value ',val,'.'
    else
      write(*,*) 'Rank ',rank,' did not successfully receive a value!'
    end if
  end if

  call MPI_Finalize(error)
end
