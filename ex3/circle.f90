! "circle.f90" - An MPI ring example, in which each process sends a 
! message to the right and receives a message from its left
!
!
!     (0) -> (1) -> ... ... -> (p)
!      ^                        |
!      |________________________|
!
!
! The code works for small size of message, but may fail for large
! ones. One should play around with n, the size of message buffer.
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
   integer:: rstat(MPI_STATUS_SIZE)
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

   call MPI_Send(smsg,n,MPI_REAL8,right,tag,MPI_COMM_WORLD,ierr)
   call MPI_Recv(rmsg,n,MPI_REAL8,left,tag,MPI_COMM_WORLD,rstat,ierr)
   print('("Rank",i3,": sent ",f5.1,", received",f5.1)'), &
      rank,smsg(1),rmsg(1)

   call MPI_Finalize(ierr)
end program circle
