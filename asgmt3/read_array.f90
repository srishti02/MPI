program read_array_in_binary
   implicit none
   integer:: i, j, m, n
   real, allocatable:: a(:,:), b(:)
   integer:: argc
   character(80):: args(10)

   argc = iargc()
   if (argc == 0) then
      print '("Please specify the file name to read")'
      stop
   endif
   call getarg(1,args(1))
   open(10,file=args(1),form='unformatted',recl=4,status='old')
   call getarg(2,args(2))
   read(args(2),*) m
   call getarg(3,args(3))
   read(args(3),*) n
   allocate(a(m,n),b(m*n))

   read(10) a(1,1)!,a(2,:),a(3,:),a(4,:),a(5,:),a(6,:),a(7,:),a(8,1:7)
   !read(10) a
   print '(/"A:")'
   do i=1,min(m,20)
      do j=1,min(n,12)
         write(*,'(f5.1)',advance='no') a(i,j)
      enddo
      write(*,*)
   enddo
   close(10)
end program read_array_in_binary
