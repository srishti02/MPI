// Copyright (C) 2006-2018 Western University
//
// This files contains routines that initialize variables and data.
// For MPI version, "ghost points" - an extra point is added to each end
// of the grid line, e.g. on the following 11-point grid line, a ghost
// point 'o' is added to each end
//
//        1  2  3  4  5  6  7  8  9 10 11	N points
//     o--*--*--*--*--*--*--*--*--*--*--*--o
//    -1  0  1  2  3  4  5  6  7  8  9 10 11	0-start counter
//
//     0  1  2  3  4  5  6  7  8  9 10 11 12	N+2 points, 0-start index
//
// Ge Baolai <bge@sharcnet.ca>
// Faculty of Science
// Western University
// SHARCNET | Compute Canada - Calcul Canada

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "solve.h"

int rank = 0; // Used as dummy var for serial case

#ifdef USE_MPI
/* Global data */
MPI_Comm comm = MPI_COMM_WORLD;
int tag = MPI_ANY_TAG;
int num_procs;
//int rank;
int above;
int below;
#endif /* USE_MPI */

static void init_mpi(int *argc, char ***argv);
static void create_local_storage(void);

/* 
 * Initialize variables and data
 */
void init(int *pargc, char ***pargv)
{

    // Initialize MPI environment and variables
    init_mpi(pargc, pargv);

    // Create storage spaces and associated pointers
    create_local_storage();

    // Set initial condition
    set_initial_cond();

    return;
}

/*
 * Initialize MPI environment
 */
static void init_mpi( int *pargc, char ***pargv)
{
#ifdef USE_MPI
    int i;

    /* Initialize MPI based around the processes split along y dimension */
    MPI_Init(pargc, pargv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    above = rank + 1;
    if (above == num_procs) above = MPI_PROC_NULL;
    below = rank - 1;
    if (below < 0) below = MPI_PROC_NULL;

    /* Local interior counts */
    ny = (nyg + rank) / num_procs;

    /* Each rank gets the partition sizes */
    if (NULL == (nys = (int *)malloc(sizeof(int)*num_procs)))
    {
        perror(strerror(errno));
        abort();
    }
    MPI_Allgather(&ny,1,MPI_INT,nys,1,MPI_INT,MPI_COMM_WORLD);
#ifdef TRACE
    printf("Rank %d: block points\n", rank);
    for (i=0; i<num_procs; i++)
        printf(" %d", nys[i]);
    printf("\n");
#endif /* TRACE */
#endif /* USE_MPI */

    return;
}

static void create_local_storage(void)
{
    int i, j;
    double x, x0, y, y0;
    int nx1, ny1;
    int nxg1, nyg1;
    
    // For MPI code, ny is local    
    nx1 = nx + 2; // Extra "ghost point" at each end
    ny1 = ny + 2; // Extra "ghost point" at each end

#ifdef USE_MPI
    // Rank 0 needs to hold the whole array for output
    if (0 == rank)
        ny1 = nyg + 2;
#endif

    // Set storages for time stepping solutions
    if( !(ws_uold = (double *)calloc(nx1*ny1, sizeof(double))) ) goto err;
    if( !(ws_unew = (double *)calloc(nx1*ny1, sizeof(double))) ) goto err;
    memset(ws_uold,0,sizeof(double)*nx1*ny1);
    memset(ws_unew,0,sizeof(double)*nx1*ny1);
    if( !(uold = (double **)malloc(sizeof(double *)*ny1)) ) goto err;
    if( !(unew = (double **)malloc(sizeof(double *)*ny1)) ) goto err;

    // Map "2D" array to 1D consecutive memory locations
    uold[0] = ws_uold;
    unew[0] = ws_unew;
    for (i = 1; i < ny1; ++i)
        uold[i] = uold[i-1] + nx1;
    for (i = 1; i < ny1; ++i)
        unew[i] = unew[i-1] + nx1;

    // Compute global coordinates once for output (plots)
    nxg1 = nxg + 2; // Extra "ghost point" at each end
    nyg1 = nyg + 2; // Extra "ghost point" at each end
    if( !(xc = (double *)malloc(sizeof(double)*nxg1)) ) goto err;
    if( !(yc = (double *)malloc(sizeof(double)*nyg1)) ) goto err;

    x0 = -dx; // Ghost point left to the left boundary
    y0 = -dy; // Ghost point below the bottom boundary
    for (i = 0; i < nxg1; ++i)
        xc[i] = x0 + i*dx;
    for (i = 0; i < nyg1; ++i)
        yc[i] = y0 + i*dy;

    // Set initial condition
    set_initial_cond();

    return;

err:
    perror(strerror(errno));
    abort();
}
