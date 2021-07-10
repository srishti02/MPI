/*
 * Copyright (C) 2006-2018 Western University
 * Ge Baolai <gebaolai@gmail.com>
 * Faculty of Science
 * Western University
 * SHARCNET | Compute Ontario | Compute Canada
 */
#ifndef _SOLVE_H
#define _SOLVE_H

#define NX_MAX 1025
#define NY_MAX 1025

#ifdef USE_MPI
#include <mpi.h>
#endif /* USE_MPI */

extern int nx, ny, nt;          /* nx, ny, nt, # of sampling times */
extern int nxg, nyg;            /* Global nx, ny, for MPI version only */
extern int *nxs, *nys;          /* Points of each partition */
extern double a;                /* a */
extern double dx, dy, dt;       /* dx, dy, dt, sampling interval */
extern double *ws_uold, *ws_unew;
extern double **uold, **unew;
extern double *xc, *yc;
extern int snapshot_freq;
extern double dx2, dy2;

extern int xoff,yoff;           /* initial offset -- used by MPI */

#ifdef USE_MPI
extern MPI_Comm comm;
extern int tag;
extern int num_procs;
extern int rank;
extern int above;
extern int below;
#endif /* USE_MPI */

extern void input(void);
extern void output(double, int);
extern void init(int *argc, char ***agv);
extern void fin(void);
extern void solve(void);
extern void aswap(double ***, double ***);

extern void set_initial_cond(void);
extern void set_boundary_cond(double t);

extern int global_yindex(int i, int rank, int *ns);
extern int global_yindex_start(int rank, int *ns);

#endif /* _SOLVE_H */
