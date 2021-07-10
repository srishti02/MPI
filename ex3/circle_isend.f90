! "circle_isend.f90" - An MPI ring example, in which each process sends a 
! message to the right through a non-blocking send and receives a message
! from its left
!
!     (0) -> (1) -> ... ... -> (p)
!      ^                        |
!      |________________________|
!
! Copyriht(C) 2002-2020 Western University
!
! Ge Baolai <bge@sharcnet.ca>
! Western University
! SHARCNET | Compute Ontario | Compute Canada
! London, Ontario, Canada N6A 5B7

program circle
   use mpi
   implicit none
   integer:: rank, num_procs, ierr
   integer:: left, right
   integer:: tag = 1
   integer:: n = 10
   real(8), allocatable:: smsg(:), rmsg(:)
   integer:: rstats(MPI_STATUS_SIZE,2), reqs(2)
   integer:: argc
   character(80):: argv(10)

   call MPI_Init(ierr)
   call MPI_Comm_size(MPI_COMM_WORLD,num_procs,ierr)
   call MPI_Comm_rank(MPI_COMM_WORLD,rank,ierr)

   if (0 == rank) then
      argc = iargc()
      if (1 == argc) then
         call getarg(1,argv(1))
         read(argv(1),*) n
      endif
   endif
   call MPI_Bcast(n,1,MPI_INT,0,MPI_COMM_WORLD,ierr);
   allocate(smsg(n),rmsg(n))
   smsg(1) = rank

   left = rank - 1
   if (left < 0) left = num_procs - 1
   right = rank + 1
   if (right == num_procs) right = 0

   call MPI_Isend(smsg,n,MPI_REAL8,right,tag,MPI_COMM_WORLD,reqs(1),ierr)
   call MPI_Irecv(rmsg,n,MPI_REAL8,left,tag,MPI_COMM_WORLD,reqs(2),ierr)
   print('("Rank",i3,": sent ",i3,", received",i3)'), &
      rank,integer(smsg(1)),integer(rmsg(1))

   call MPI_Waitall(2,reqs,rstats,ierr)

   call MPI_Finalize(ierr)
end program circle
