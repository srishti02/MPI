// "chain_bidir_sendrecv.c" - An MPI chain example, in which each process
// sends and receives a message to/from the right and sends a receives a 
// message to/from its left using sendrecv call
//
//     (0) <=> (1) <=> ... ... <=> (p)
//
// This example tests a shift operation across a chain.
//
// Copyriht(C) 2002-2020 Western University
//
// Ge Baolai <bge@sharcnet.ca>
// Western University
// SHARCNET | Compute Ontario | Compute Canada
// London, Ontario, Canada N6A 5B7
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, num_procs, ierr;
    int left, right;
    int rtag = 1, ltag = 2;
    int n = 10;
    double *rsmsg, *rrmsg, *lsmsg, *lrmsg;
    MPI_Status rstat;

    // Initialize the MPI pool
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    // Rank 0 to get the message size and broadcast it
    if (0 == rank)
        if (2 == argc)
            n = atoi(argv[1]);		
    MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
    rsmsg = (double *)malloc(sizeof(double)*n);
    rrmsg = (double *)malloc(sizeof(double)*n);
    lsmsg = (double *)malloc(sizeof(double)*n);
    lrmsg = (double *)malloc(sizeof(double)*n);
    rsmsg[0] = rank;
    lsmsg[0] = rank;

    // Label the neighbors' ranks
    left = rank - 1;
    if (left < 0) left = MPI_PROC_NULL;
    right = rank + 1;
    if (right == num_procs) right = MPI_PROC_NULL;

    // Send a message to the right and receive from the left
    MPI_Sendrecv(rsmsg,n,MPI_DOUBLE,right,rtag,
        lrmsg,n,MPI_DOUBLE,left,rtag,MPI_COMM_WORLD,&rstat);
    printf("Rank %d: sent to right(%d), received from left(%d)\n",
        rank, right,(int)lrmsg[0]);

    // Send a message to the left and receive from the right
    MPI_Sendrecv(lsmsg,n,MPI_DOUBLE,left,ltag,
        rrmsg,n,MPI_DOUBLE,right,ltag,MPI_COMM_WORLD,&rstat);
    printf("Rank %d: sent to left(%d), received from right(%d)\n",
        rank, left,(int)lrmsg[0]);

    // Finalize (unnecessary)
    MPI_Finalize();

    return 0;
}
