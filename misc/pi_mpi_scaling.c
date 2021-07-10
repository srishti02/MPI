// "pi_mpi_scaling.c" - Compute pi by approximating the area under the
// curve f(x) = 4 / (1 + x*x) between 0 and 1.
//
// Time the code by changing the number of processors and number of
// subintervals.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int done = 0, n=1000000, myid, numprocs, i;
    double start_time, end_time;
    double PI25DT = 3.141592653589793238462643;
    double mypi, pi, h, sum, x;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);

    start_time = MPI_Wtime();

    if (myid == 0) {
        printf("This program uses %d processes\n", numprocs);
        if (argc == 2)
            n = atoi(argv[1]);
        printf("The number of intervals = %d \n", n);
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  
    h   = 1.0 / (double) n;
    sum = 0.0;
    for (i = myid + 1; i <= n; i += numprocs) {
        x = h * ((double)i - 0.5);
        sum += 4.0 / (1.0 + x*x);
    }
    mypi = h * sum;
    
    MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    end_time = MPI_Wtime();
    
    if (myid == 0){
        printf("pi is approximately %.16f, Error is %.16f\n",
            pi, fabs(pi - PI25DT));
        printf("execution time = %g\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
