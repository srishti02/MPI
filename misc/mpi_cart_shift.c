// "mpi_cart_shift.c"
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[])
{
    int coords[2];
    int rank, grid_rank, num_procs;

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    MPI_Comm grid_comm;
    int grid_size[2] = {0,0}; // Must set to 0 for modification
    int period[2] = {0,0};
    
    // Create a closest division of p x q grid given number of total procs
    MPI_Dims_create(num_procs, 2, grid_size);

    // Create a Cartesian communicator
    MPI_Cart_create(
        MPI_COMM_WORLD, // Old communicator (abr. comm)
        2,		// Dimension of the new comm
        grid_size, 	// Shape of the new comm, e.g. (2,3)
        period, 	// Periodic cond in each dim, e.g. (0,1)
        1, 		// 1 if ranking may be reordered (0 otherwise)
        &grid_comm	// Returned new communicator
    );
    MPI_Comm_rank(grid_comm, &grid_rank);

    // Get my coordinates (each rank has its mapped coordiates)
    MPI_Cart_coords(
        grid_comm, 	// The grid communicator
        grid_rank, 	// The grid rank
        2, 		// Dimension of the grid
        coords		// Returned coordinates
    );

    // Find the global ranks of neighbors
    int left, right, bottom, top;
    MPI_Cart_shift(grid_comm, 0, 1, &top, &bottom);
    printf("World rank: %d, grid coords=(%d,%d), top=%d, bottom=%d\n",
        rank, coords[0], coords[1], top, bottom);
    MPI_Cart_shift(grid_comm, 1, 1, &left, &right);
    printf("World rank: %d, grid coords=(%d,%d), left=%d, right=%d\n",
        rank, coords[0], coords[1], left, right);

    // Finalize and exit
    MPI_Finalize();
    return 0;
}

