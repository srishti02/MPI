#include <math.h>
#include "solve.h"

static double boundary_cond(double x, double y, double t);

void set_boundary_cond(double t)
{
#ifdef USE_NONTRIVIAL_BC
    int i, j, jg;

    // Set BCs on left and right edges
    for (j = 0; j <= ny; j++) {
        jg = global_yindex(0,rank,nys);
        unew[j][0] = boundary_cond(xc[0], yc[jg], t);
        jg = global_yindex(ny-1,rank,nys);
        unew[j][nx] = boundary_cond(xc[nx], yc[jg], t);
    }

    // Set BCs on top and bottom edges
    for (i = 0; i <= nx; ++i) {
        jg = global_yindex(0,rank,nys);
        unew[0][i] = boundary_cond(xc[i], yc[jg], t);
        jg = global_yindex(ny-1,rank,nys);
        unew[ny][i] = boundary_cond(xc[i], yc[jg], t);
    }
#endif /* USE_NONTRIVIAL_BC */
    return;
}

// Set boundary condition
static double boundary_cond(double x, double y, double t)
{
    double u;

    if (fabs(x) <= 1.0e-7) u = 0;
    if (fabs(x-1.0) <= 1.0e-7) u = 0;
    if (y <= 1.0e-7) u = 0;
    if (fabs(y-1.0) <= 1.0e-7) u = 0;

    return 0.0;
}
