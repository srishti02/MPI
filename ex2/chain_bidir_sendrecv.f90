! "chain_bidir_sendrecv.f90" - An MPI chain example, in which each process
! sends and receives a message to/from the right and sends a receives a 
! message to/from its left using sendrecv call
!
!     (0) <=> (1) <=> ... ... <=> (p)
!
! This example tests a shift operation across a chain.
!
! Copyriht(C) 2002-2020 Western University
!
! Ge Baolai <bge@sharcnet.ca>
! Western University
! SHARCNET | Compute Ontario | Compute Canada
! London, Ontario, Canada N6A 5B7

program chain_bidir_sendrecv
   use mpi
   implicit none
   integer:: rank, num_procs, ierr
   integer:: left, right
   integer:: rtag = 1, ltag = 2
   integer:: n = 10
   real(8), allocatable:: rsmsg(:), rrmsg(:), lsmsg(:), lrmsg(:)
   integer:: rstat(MPI_STATUS_SIZE)
   integer:: argc
   character(80):: argv(10)

   call MPI_Init(ierr)
   call MPI_Comm_size(MPI_COMM_WORLD,num_procs,ierr)
   call MPI_Comm_rank(MPI_COMM_WORLD,rank,ierr)

   if (0 == rank) then
      argc = command_argument_count()            
      if (1 == argc) then
         call get_command_argument(1,argv)              
         read(argv(1),*) n
      endif
   endif      

   call MPI_Bcast(n,1,MPI_INT,0,MPI_COMM_WORLD,ierr)
   allocate(rsmsg(n),rrmsg(n),lsmsg(n),lrmsg(n))
   rsmsg(1) = rank
   lsmsg(1) = rank

   left = rank - 1
   if (left < 0) left = MPI_PROC_NULL
   right = rank + 1
   if (right == num_procs) right = MPI_PROC_NULL

   ! Send a message to the right and receive from the left
   call MPI_Sendrecv(rsmsg,n,MPI_DOUBLE,right,rtag, &
      lrmsg,n,MPI_DOUBLE,left,rtag,MPI_COMM_WORLD,rstat,ierr)
   print('("Rank",i3,": sent",i4,", received",i3," to/from right rank",i3)'), &
      rank,int(rsmsg(1)),int(lrmsg(1)),right

   ! Send a message to the left and receive from the right
   call MPI_Sendrecv(lsmsg,n,MPI_DOUBLE,left,ltag, &
      rrmsg,n,MPI_DOUBLE,right,ltag,MPI_COMM_WORLD,rstat,ierr)
   print('("Rank",i3,": sent",i4,", received",i3," to/from right rank",i3)'), &
      rank,int(lsmsg(1)),int(rrmsg(1)),left

   call MPI_Finalize(ierr)
end program chain_bidir_sendrecv
