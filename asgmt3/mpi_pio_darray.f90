! 'mpi_pio.darray.f90' - Test driver for writing an 8x8 matrix
! distributed on 2x2 processes to a single file by each process. 
! The 8x8 matrix is partitioned evenly into 2x2 blocks as follows:
! 
!     1.0  9.0 17.0 25.0 | 33.0 41.0 49.0 57.0
!                        |
!     2.0 10.0 18.0 26.0 | 34.0 42.0 50.0 58.0
!                        |
!     3.0 11.0 19.0 27.0 | 35.0 43.0 51.0 59.0
!                        |
!     4.0 12.0 20.0 28.0 | 36.0 44.0 52.0 60.0
!                        |
!     -------------------+--------------------
!                        |
!     5.0 13.0 21.0 29.0 | 37.0 45.0 53.0 61.0
!                        |
!     6.0 14.0 22.0 30.0 | 38.0 46.0 54.0 62.0
!                        |
!     7.0 15.0 23.0 31.0 | 39.0 47.0 55.0 63.0
!                        |
!     8.0 16.0 24.0 32.0 | 40.0 48.0 56.0 64.0
!
! Each process holds a block and assembles it into the global matrix
! written into the right positions in the same file.
!
! For demo purpose, the distributed array on each process is created
! from the whole array, which is generated on each process for
! convenience.
!
! Copyright(C) 2002-2018 Western University
! Ge Baolai
! gebaolai@gmail.com
! Western University
! SHARCNET | Compute/Calcul Ontario | Compute/Calcul Canada

program mpi_pio_darray
   use mpi
   implicit none
   integer, parameter:: n=8
   real:: a(n,n), a_local(n/2,n/2)
   integer:: i, j, is, ie, js, je, n_local
   integer:: gsize(2), lsize(2), start_indices(2)
   integer:: rank, row_rank, col_rank, num_procs, ierr
   integer(kind=MPI_OFFSET_KIND):: disp=0
   integer:: fh !C: MPI_File fh
   integer:: filetype !C: MPI_Datatype filetype
   integer:: stat(MPI_STATUS_SIZE) !C: MPI_Status stat

   ! Initialize the MPI environment
   call MPI_Init(ierr)
   call MPI_Comm_size(MPI_COMM_WORLD,num_procs,ierr)
   call MPI_Comm_rank(MPI_COMM_WORLD,rank,ierr)
   if (num_procs /= 4) then
      if (rank == 0) print '("Number of process must be 4")'
      call MPI_Finalize(ierr)
      stop
   endif

   ! Just create the whole matrix for convenience
   do j=1,n
      do i=1,n 
         !a(i,j) = (j-1)*n+i 
         a(i,j) = (i-1)*n+j 
      enddo
   enddo

   ! Create the local array by rank
   n_local = n / 2
   row_rank = rank / 2
   col_rank = mod(rank,2)
   is = row_rank*n_local + 1
   ie = is + n_local - 1
   js = col_rank*n_local + 1
   je = js + n_local - 1
   a_local(1:n_local,1:n_local) = ! ** Fill this out **

   ! Display the global and local matrix for checking
   if (row_rank == 1 .and. col_rank == 1) then
      print '(/"A(global):")'
      do i=1,n
         do j=1,n
            write(*,'(f5.1)',advance='no') a(i,j)
         enddo
         print '(/)'
      enddo
      print '(/"A_local on rank",i3,":")', rank
      do i=1,n_local
         do j=1,n_local
            write(*,'(f5.1)',advance='no') a_local(i,j)
         enddo
         print '(/)'
      enddo
   endif

   ! Get the assembly parameters right
   gsize = [n,n]                        ! Global matrix size
   lsize = [n_local,n_local]            ! Local submatrix size
   start_indices(1) = row_rank*n_local  ! Global 0-start row-index
   start_indices(2) = col_rank*n_local  ! Global 0-start col-index

   ! Create subarray type (what on earth shall one do this?)
   call MPI_Type_create_subarray(2,gsize,lsize,start_indices,&
      MPI_ORDER_FORTRAN,MPI_REAL,filetype,ierr)
   call MPI_Type_commit(filetype,ierr)

   ! Each process write the local array to the file
   call MPI_File_open(MPI_COMM_WORLD,'a8x8.dat',&
      MPI_MODE_CREATE+MPI_MODE_WRONLY,MPI_INFO_NULL,fh,ierr)
   call MPI_File_set_view(fh,disp,MPI_REAL,filetype,'native',&
      MPI_INFO_NULL,ierr)
   call MPI_File_write_all(fh,a_local,n_local*n_local,&
      MPI_REAL,stat,ierr)
   call MPI_File_close(fh,ierr)

   ! Finilize the MPI environment
   call MPI_Finalize(ierr)
end program mpi_pio_darray
