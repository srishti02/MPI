Assignment 3

WHAT TO DO

Write an 8x8 array distributed on 2x2 processes to a file using MPI parallel 
I/O functions. The 8x8 array is partitioned evenly into 2x2 blocks as follows:

     1.0  9.0 17.0 25.0 | 33.0 41.0 49.0 57.0
                        |
     2.0 10.0 18.0 26.0 | 34.0 42.0 50.0 58.0
                        |
     3.0 11.0 19.0 27.0 | 35.0 43.0 51.0 59.0
                        |
     4.0 12.0 20.0 28.0 | 36.0 44.0 52.0 60.0
                        |
     -------------------+--------------------
                        |
     5.0 13.0 21.0 29.0 | 37.0 45.0 53.0 61.0
                        |
     6.0 14.0 22.0 30.0 | 38.0 46.0 54.0 62.0
                        |
     7.0 15.0 23.0 31.0 | 39.0 47.0 55.0 63.0
                        |
     8.0 16.0 24.0 32.0 | 40.0 48.0 56.0 64.0

Each process holds a block and assembles it into the global matrix
written into the right positions in the same file.

Complete the code in mpi_pio_darray.c (or mpi_pio_darray.f90). Edit it to
complete the missing pieces indicated by the comments "** Fill this out **" 
in the code.

FILES

Two files for editing are provided: mpi_pio_darray.c and mpi_pio_darray.f90. 
You may choose either of them. The output of the global array "a8x8.bin" is 
in binary format. You may use the C code read_array.c to check the result.

TESTING

To compile the code, use command

    mpicc mpi_pio_darray.c

To run the code, use command

    mpirun -n 4 ./a.out

To check the result, compile and run the C code read_array.c

    gcc read_array.c -o read_array
    ./read_array a8x8 8 8

The Fortran code will generate a file "a8x8.dat". You should be able to 
read it using read_array as well

    ./read_array a8x8.dat 8 8

But note that the output is a transpose of the array, as Fortran uses the 
column major for storing data.

Submit your completed source code only.
