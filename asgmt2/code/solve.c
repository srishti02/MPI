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
// Ge Baolai <bge@sharcnet.ca>
// Faculty of Science
// Western University
// SHARCNET | Compute Ontario | Compute Canada 

#include "solve.h"
#include "timer.h"

void solve(void)
{
    int i, j, k;
    double t, uxx, uyy;

    t = 0.0;
    output(t, 0);

    // Time integration from t = 0 to tend
    for (k = 1; k <= nt; ++k) 
    {
        // Compute new values of u for current time (for MPI ny is local)
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

        // Swap values in unew and uold 
        aswap(&unew, &uold);
    }

    return;
}
