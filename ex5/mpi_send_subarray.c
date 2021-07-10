// "mpi_send_subarray.c" - This program demonstrates how to send a subarray
// (noncontiguous data). Rank 1 sends an updated subarray to Rank 0 and
// Rank 0 prints the content of it. It is assumed that both rank 0 and
// rank 1 have the storage of the same size for the array.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define SIZE 8

int main(int argc, char* argv[])
{
    int i, j, n=SIZE, rank;
    float a[64] = {0};	// To hold an 8x8 matrix
 
    // Initialize the MPI state
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    
    // Rank 1 updates array a
    if (1 == rank)
    {
        printf("\nA on Rank 1:\n");
        for (i=0;i<n;i++)
        {		
            for (j=0;j<n;j++)
            {		    
                a[i*n+j] = i*n+j+1;
                printf("%5.1f ", a[i*n+j]);
            }		
	    printf("\n");
	}
        fflush(stdout);	
    }
    MPI_Barrier(MPI_COMM_WORLD); // For the integrity of display only
 
    // Create a new data type
    MPI_Datatype datatype_subarray ;	   
    int gdims[2] = {SIZE,SIZE};
    int sdims[2] = {4,4};	
    int istart[2] = {0,4};

    MPI_Type_create_subarray(
        2,		// Dimension of the array
        gdims,		// Dimension of the global array
        sdims,		// Dimension of the 
        istart,		// Start-indices of the global array 
        MPI_ORDER_C,	// Order of array storage
        MPI_REAL,	// Type of array elements
        &datatype_subarray	// New data type	
    );	  
    MPI_Type_commit(&datatype_subarray);

    // Rank 0 checks received content from Rank 1
    if (1 == rank)
    {
        MPI_Send(a,1,datatype_subarray,0,999,MPI_COMM_WORLD);
    }
    else
    {
        MPI_Status status;	    
        MPI_Recv(a,1,datatype_subarray,1,999,MPI_COMM_WORLD,&status);    
        printf("\nA on Rank 0:\n");
        for (i=0;i<n;i++)
        {		
            for (j=0;j<n;j++)		
                printf("%5.1f ", a[i*n+j]);
	    printf("\n");
	}
    }

    // Shutdown
    MPI_Type_free(&datatype_subarray);
    MPI_Finalize();

    return 0;
}
