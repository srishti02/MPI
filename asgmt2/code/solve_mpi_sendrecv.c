// Copyright (C) 2006-2018 Western University
//
// This subroutine solves two dimensional heat equation 
//
//   u  = D(u   + u  ), u = u(x,y), x, y \in [0,1]x[0,1]
//    t      xx    yy
//
// using explicit finite difference scheme on an n  x n  uniform 
//                                                x    y
// rectangular grid. 
//
// The code below is for the calculation of u on a subdomain assigned to
// a process. The original domain is decomposed in y-direction into N
// subdomain, where N is equal to the number of processes. Each subdomain
// needs to exchange data on the boundary with its upper and lower neighbors
// with ranks labelled "above" and "below" respectively, as illustrated 
// shown
//
//                                 
//                  |             above             |
//                  +-------------------------------+
//
//                  +-------------------------------+
//                  |                               |
//                  |              me               |
//                  |                               |
//                  +-------------------------------+
//
//                  +-------------------------------+
//                  |             below             |
//
//                  
// The rank of the upper and lower neighbors are defined as global variables
// in init.c already, as shown below
//
//    above = rank + 1;
//    if (above == num_procs) above = MPI_PROC_NULL;
//    below = rank - 1;
//    if (below < 0) below = MPI_PROC_NULL;
//
// You need to fill the missing ranks using these two variables in the 
// MPI_Sendrecv() calls below. That's all!
//
// Ge Baolai <bge@sharcnet.ca>
// Faculty of Science
// Western University
// SHARCNET | Compute Ontario | Compute Canada 

#include "solve.h"
#include "timer.h"

#define TAG_A 1
#define TAG_B 2

void solve(void)
{
    MPI_Status stas[4];
    MPI_Request reqs[4];

    int i, j, k;
    double t, uxx, uyy;

    t = 0.0;
    output(t, 0);

    // Perform time integration from t = 0 to tend by each rank
    for (k = 1; k <= nt; ++k) 
    {
        // Exchange boundary data with neighbors in upward directions
        MPI_Sendrecv(
            &uold[ny][0],
            (nx+2),
            MPI_DOUBLE, 
               above,	// Fill an appropriate rank 
	    TAG_A, 
            &uold[0][0], 
            (nx+2), 
            MPI_DOUBLE, 
               below,	// Fill an appropriate rank
            TAG_A, 
            comm, 
            &stas[1]
        );

	// Exchange boundary data with neighbors in downward directions
        MPI_Sendrecv(
            &uold[1][0], 
            (nx+2), 
            MPI_DOUBLE, 
               below, 	// Fill an appropriate rank
            TAG_B, 
            &uold[ny+1][0], 
            (nx+2), 
            MPI_DOUBLE, 
               above,  	// Fill an appropriate rank
            TAG_B, 
            comm, 
            &stas[0]
        );

        // Compute new values of u for current time (ny is local)
        for (j = 1; j <= ny; ++j) 
            for (i = 1; i <= nx; ++i) {
                uxx = (uold[j][i+1] - 2.0*uold[j][i] + uold[j][i-1])/dx2;
                uyy = (uold[j+1][i] - 2.0*uold[j][i] + uold[j-1][i])/dy2;
                unew[j][i] = dt*a*(uxx + uyy) + uold[j][i];
            }

        // Update boundary points with boundary condition
        set_boundary_cond(t);

        // Output snapshot of solution
        t += dt;
        output(t, k);

        // Swap storage unew and uold 
        aswap(&unew, &uold);

        // Ensure every process has done the update
        MPI_Barrier(MPI_COMM_WORLD);
    }

    return;
}
