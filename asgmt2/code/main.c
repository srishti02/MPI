/*
 * This example demonstrates the solution of heat transfer equation 
 *
 *   u  = a(u   + u  ), u = u(x,y), x, y \in [0,1]x[0,1]
 *    t      xx    yy
 *
 * using explicit scheme with finite difference method on an
 * (n + 1) x (n + 1) uniform, rectangular grid.
 *   x         y
 *
 * Copyright (C) 2006i-2018 Western University
 *
 * Ge Baolai <gebaolai@gmail.com>
 * Faculty of Science
 * Western University
 * SHARCNET | Compute Ontario | Compute Canada
 */
#include <stdio.h>
#include "solve.h"

int main( int argc, char *argv[] )
{
    input();
    init(&argc, &argv);
    solve();
    fin();
    return 0;
}
