// "circle_ssend.c" - An MPI ring example, in which each process sends a 
// message to the right through a synchronous, blocking send and receives
// a message from its left
//
//     (0) -> (1) -> ... ... -> (p)
//      ^                        |
//      |________________________|
//
// This code is likely to enter a deadlock.
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
    int rank, num_procs;
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
    if (left < 0) left = num_procs - 1;
    right = rank + 1;
    if (right == num_procs) right = 0;

    MPI_Ssend(smsg,n,MPI_DOUBLE,right,tag,MPI_COMM_WORLD);
    MPI_Recv(rmsg,n,MPI_DOUBLE,left,tag,MPI_COMM_WORLD,&rstat);
    printf("Rank %d: sent %d, received %d\n",rank,(int)smsg[0],(int)rmsg[0]);

    MPI_Finalize();

    return 0;
}
