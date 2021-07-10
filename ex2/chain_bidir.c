// "chain_bidir.c" - An MPI chain example, in which each process sends a 
// message to the right and receives a message from its left as illustrated
// below with p processes.
//
//     (0) <=> (1) <=> ... ... <=> (p)
//
// The code works for small size of message, but may fail for large
// ones. One should play around with n, the size of message buffer.
// 
// Copyriht(C) 2002-2021 Western University
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
    int tag = 1;
    int n = 10;
    double *smsg, *rmsg;
    MPI_Status rstat;

    // Initialize the MPI pool
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    // Rank 0 to get message size and broadcast it to all
    if (0 == rank)
        if (2 == argc)
            n = atoi(argv[1]);		
    MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);

    // Allocate enough space for messages
    smsg = (double *)malloc(sizeof(double)*n);
    rmsg = (double *)malloc(sizeof(double)*n);
    smsg[0] = rank;

    // Label the neighbors' ranks
    left = rank - 1;
    if (left < 0) left = MPI_PROC_NULL;
    right = rank + 1;
    if (right == num_procs) right = MPI_PROC_NULL;

    // Perform communications in left to right direction
    MPI_Send(smsg,n,MPI_DOUBLE,right,tag,MPI_COMM_WORLD);
    MPI_Recv(rmsg,n,MPI_DOUBLE,left,tag,MPI_COMM_WORLD,&rstat);
    printf("Rank %d: sent to right(%d), received from left(%d)\n",
        rank, right, left);
    
    // Perform communications in right to left direction
    MPI_Send(smsg,n,MPI_DOUBLE,left,tag,MPI_COMM_WORLD);
    MPI_Recv(rmsg,n,MPI_DOUBLE,right,tag,MPI_COMM_WORLD,&rstat);
    printf("Rank %d: sent to left(%d), received from right(%d)\n",
        rank, right, left);

    // Finalize
    MPI_Finalize();

    return 0;
}
