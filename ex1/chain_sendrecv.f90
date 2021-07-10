! "chain_sendrecv.f90" - An MPI chain example, in which each process
! sends a message to the right and receives a message from its left
! in a combined call
!
!     (0) -> (1) -> ... ... -> (p)
!
! Copyriht(C) 2002-2020 Western University
!
! Ge Baolai <bge@sharcnet.ca>
! Western University
! SHARCNET | Compute Ontario | Compute Canada
! London, Ontario, Canada N6A 5B7


program chain
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
   if (left < 0) left = MPI_PROC_NULL
   right = rank + 1
   if (right == num_procs) right = MPI_PROC_NULL

   call MPI_Sendrecv(smsg,n,MPI_REAL8,right,tag,&
      rmsg,n,MPI_REAL8,left,tag,MPI_COMM_WORLD,rstat,ierr)
   print('("Rank",i3,": sent ",f5.1,", received",f5.1)'), &
      rank,smsg(1),rmsg(1)

   call MPI_Finalize(ierr)
end program chain
