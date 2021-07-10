// "chain_ssend.c" - An MPI chain example, in which each process sends a 
// message to the right and receives a message from its left as illustrated
// below with p processes
//
//     (0) -> (1) -> ... ... -> (p)
//
// The code works for small size of message, but may fail for large
// ones. One should play around with n, the size of message buffer.
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
    int tag = 1;
    int n = 10;
    double *smsg, *rmsg;
    MPI_Status rstat;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    if (0 == rank)
        if (2 == argc)
            n = atoi(argv[1]);		
    MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
    smsg = (double *)malloc(sizeof(double)*n);
    rmsg = (double *)malloc(sizeof(double)*n);
    smsg[0] = rank;

    left = rank - 1;
    if (left < 0) left = MPI_PROC_NULL;
    right = rank + 1;
    if (right == num_procs) right = MPI_PROC_NULL;

    MPI_Ssend(smsg,n,MPI_DOUBLE,right,tag,MPI_COMM_WORLD);
    MPI_Recv(rmsg,n,MPI_DOUBLE,left,tag,MPI_COMM_WORLD,&rstat);
    printf("Rank %d: sent %d, received %d\n",rank,(int)smsg[0],(int)rmsg[0]);

    MPI_Finalize();

    return 0;
}
