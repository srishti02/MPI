// "chain_bidir_3procs.c" - An MPI chain of 3, in which each process sends a 
// message to the right and receives a message from its left as illustrated
// below with p processes.
//
//     (0) <=> (1) <=> (2)
//
// The code should not run into a deadlock.
//
// Change the order of MPI_Send and MPI_Recv in the right process and see
// what will happen. Use different message buffer size in your experiment.
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
    int left=0, middle=1, right=2;
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

    // Note the following are mutually exclusive
    if (rank == left)
    {
        // Perform communications in left to right direction
        MPI_Send(smsg,n,MPI_DOUBLE,middle,tag,MPI_COMM_WORLD);
        MPI_Recv(rmsg,n,MPI_DOUBLE,middle,tag,MPI_COMM_WORLD,&rstat);
        printf("Rank %d: sent to right(%d), received from right(%d)\n",
            rank, right, left);
    }

    if (rank == middle)
    {
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
    }	

    if (rank == right)
    {
        // Perform communications in right to left direction
        MPI_Recv(rmsg,n,MPI_DOUBLE,middle,tag,MPI_COMM_WORLD,&rstat);
        MPI_Send(smsg,n,MPI_DOUBLE,middle,tag,MPI_COMM_WORLD);
        printf("Rank %d: received from left(%d), sent to left(%d)\n",
            rank, right, left);
    }

    // Finalize
    MPI_Finalize();

    return 0;
}
