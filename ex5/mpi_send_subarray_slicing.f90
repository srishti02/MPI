! "mpi_send_subarray_slicing.c" - This program demonstrates how to send a 
! subarray (noncontiguous data). Rank 1 sends an updated subarray to Rank 0 
! and Rank 0 prints the content of it. It is assumed that both rank 0 and
! rank 1 have the storage of the same size for the array.

#define SIZE 8

program main
   use mpi_f08        
   implicit none
   integer:: i, j, n=SIZE, rank, ierr
   real:: a(8,8) ! Only initialized on Rank 1
#ifdef USE_BUFFER
   real:: buf(16)
#endif
   type(MPI_Status):: stat
 
   ! Initialize the MPI state
   call MPI_Init(ierr)
   call MPI_Comm_rank(MPI_COMM_WORLD,rank,ierr)
   
   ! Rank 1 updates array a
   if (1 == rank) then
      print *, 'A on Rank 1:'
      do i = 1, n
         do j = 1, n
            a(i,j) = (i-1)*n+j
         enddo
         print '(8f5.1)', a(i,:)
      enddo
   endif
   call MPI_Barrier(MPI_COMM_WORLD,ierr) ! For the integrity of display only
 
   ! Rank 0 checks received content from Rank 1
   if (1 == rank) then
      call MPI_Send(a(1:4,5:8),16,MPI_REAL,0,999,MPI_COMM_WORLD,ierr)
   else
#ifndef USE_BUFFER
      call MPI_Recv(a(1:4,5:8),16,MPI_REAL,1,999,MPI_COMM_WORLD,stat)   
#else          
      call MPI_Recv(buf,16,MPI_REAL,1,999,MPI_COMM_WORLD,stat)   
      a(1:4,5:8) = reshape(buf,[4,4]);
#endif 
      print *, 'A on Rank 0:'
      do i = 1, n
         print '(8f5.1)', a(i,:)
      enddo
   endif

   ! Shutdown
   call MPI_Finalize(ierr);
end program main
