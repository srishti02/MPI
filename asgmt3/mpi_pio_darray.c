// 'mpi_pio.darray.c' - Test driver for writing an 8x8 matrix
// distributed on 2x2 processes to a single file by each process. 
// The 8x8 matrix is partitioned evenly into 2x2 blocks as follows:
// 
//     1.0  9.0 17.0 25.0 | 33.0 41.0 49.0 57.0
//                        |
//     2.0 10.0 18.0 26.0 | 34.0 42.0 50.0 58.0
//                        |
//     3.0 11.0 19.0 27.0 | 35.0 43.0 51.0 59.0
//                        |
//     4.0 12.0 20.0 28.0 | 36.0 44.0 52.0 60.0
//                        |
//     -------------------+--------------------
//                        |
//     5.0 13.0 21.0 29.0 | 37.0 45.0 53.0 61.0
//                        |
//     6.0 14.0 22.0 30.0 | 38.0 46.0 54.0 62.0
//                        |
//     7.0 15.0 23.0 31.0 | 39.0 47.0 55.0 63.0
//                        |
//     8.0 16.0 24.0 32.0 | 40.0 48.0 56.0 64.0
//
// Each process holds a block and assembles it into the global matrix
// written into the right positions in the same file.
//
// For demo purpose, the distributed array on each process is created
// from the whole array, which is generated on each process for
// convenience.
//
// Copyright(C) 2002-2021 Western University
// Ge Baolai
// Western University
// SHARCNET | Compute/Calcul Ontario | Compute/Calcul Canada

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[])
{	
    int n=8;
    float a[n*n], a_local[(n/2)*(n/2)];
    int i, j, is, ie, js, je, n_local;
    int gsize[2], lsize[2], start_indices[2];
    int rank, row_rank, col_rank, num_procs;
    MPI_Offset disp=0;
    MPI_File fh;
    MPI_Datatype filetype;
    MPI_Status stat;

    // Initialize the MPI environment
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    if (num_procs != 4) {
        if (rank == 0) printf("Number of process must be 4");
        MPI_Abort(MPI_COMM_WORLD,0);
    }

    // The whole matrix is created for your convenience, try not to use it
    for (i=0; i<n; i++)
        for(j=0; j<n; j++)
            a[i*n+j] = i*n+j+1; 

    // Create the local array by rank
    n_local = n / 2;
    row_rank = rank / 2;
    col_rank = rank % 2;
    is = row_rank*n_local;
    ie = is + n_local - 1;
    js = col_rank*n_local;
    je = js + n_local - 1;
    printf("Rank : %d",rank);
    printf("%d, %d,%d,%d,%d,%d,%d,%d",n,n_local,row_rank,col_rank,is,ie,js,je);
    for (i=0; i<n_local; i++)
      for (j=0; j<n_local; j++)
        a_local[i*n_local + j] = ;// ** Fill this out **

    // Display the global and local matrix for checking
    if (row_rank == 0 && col_rank == 1) {
      printf("\nA(global):\n");
      for (i=0; i<n; i++) {
        for (j=0; j<n; j++)
          printf("%5.1f", (float)(i*n+j+1));
        printf("\n\n");
      }
      printf("\nA_local on rank %d:\n",rank);
      for (i=0; i<n_local; i++) {
        for (j=0; j<n_local; j++)
          printf("%5.1f", a_local[i*n_local+j]);
        printf("\n\n");
      }
    }

    // Get the assembly parameters right
    gsize[0] = n; gsize[1] = n;		// Global matrix size
    lsize[0] = n_local; lsize[1] = n_local; // Local submatrix size
    start_indices[0] = row_rank*n_local;// Global 0-start row-index
    start_indices[1] = col_rank*n_local;// Global 0-start col-index
    
    // Create subarray type (what on earth shall one do this?)
    MPI_Type_create_subarray(2,gsize,lsize,start_indices,
        MPI_ORDER_C,MPI_FLOAT,&filetype);
    MPI_Type_commit(&filetype);

    // Each process write the local array to the file
    MPI_File_open(MPI_COMM_WORLD,"a8x8.bin",
        MPI_MODE_CREATE|MPI_MODE_WRONLY,MPI_INFO_NULL,&fh);
    MPI_File_set_view(fh,0,MPI_FLOAT,filetype,"native",MPI_INFO_NULL);
    MPI_File_write_all(fh,a_local,n_local*n_local,MPI_FLOAT,&stat);
    MPI_File_close(&fh);

    // Finilize the MPI environment
    MPI_Finalize();
    return 0;
}
