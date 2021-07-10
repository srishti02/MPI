#include <math.h>
#include "solve.h"

static double initial_cond(double x, double y);

/*
 * Set initial condition
 */
void set_initial_cond(void)
{
    int i, j, jg, nx1, ny1;
    double x, y;

#ifndef USE_MPI
    for (j = 0; j < ny+2; ++j) {
        y = yc[j];
        for (i = 0; i < nx+2; ++i) {
            x = xc[i];
            uold[j][i] = initial_cond(x,y);
            unew[j][i] = uold[j][i];
        }
    }
#else
    nx1 = nx + 2;
    if (rank == 0) // For output, set the i.c. for the entire domain
    {	   
        ny1 = nyg + 2;	
        for (j = 0; j < ny1; ++j) {
            y = yc[j];
            for (i = 0; i < nx1; ++i) {
                x = xc[i];
                uold[j][i] = initial_cond(x,y);
                unew[j][i] = uold[j][i];
            }
        }
    }
    else // Set the i.c. for its own subdomain for calculation
    {	    
        ny1 = ny + 2;    
        for (j = 0; j < ny1; ++j) {
            jg = global_yindex_start(rank,nys) - 1 + j;
            y = yc[jg];
            for (i = 0; i < nx1; ++i) {
                x = xc[i];
                uold[j][i] = initial_cond(x,y);
                unew[j][i] = uold[j][i];
            }
        }
    }
#endif
}

/* 
 * Define initial condition
 */
static double initial_cond(double x, double y)
{
    double r1=0.22222222, r2=0.33333333, x0=0.5, y0=0.5;
    double r, f;

#ifndef USE_SPHERE
    r = sqrt((x - x0)*(x - x0) + (y - y0)*(y - y0));
    if (r1 <= r && r <= r2)
       f = 1.0/6;
    else
       f = 0.0;
#else
    r = sqrt((x - x0)*(x - x0) + (y - y0)*(y - y0));
    if (r <= r2)
       f = sqrt(r2*r2 - r*r);
    else
       f = 0.0;
    end if
#endif

    return f;
}

