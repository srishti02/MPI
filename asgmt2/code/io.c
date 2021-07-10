/*
 * I/O routines
 * Copyright (C) 2006 Ge Baolai <gebaolai@gmail.com>
 * $Log: io.f90,v $
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "solve.h"

/* Read a number per line */
#define READN( fp, data )                                               \
{                                                                       \
    char __data_buf[BUFSIZ];                                            \
    fgets(__data_buf, BUFSIZ, fp);                                      \
    data = atof(strtok(__data_buf, ", \t\n"));                          \
}

/* Read a char per line */
#define READC( fp, data )                                               \
{                                                                       \
    char __data_buf[BUFSIZ];                                            \
    char * __ptr = NULL;                                                \
    fgets(__data_buf, BUFSIZ, fp);                                      \
    __ptr = strchr(__data_buf, '\'');                                   \
    __ptr++;                                                            \
    data = *__ptr;                                                      \
}

/* Read a string per line */
#define READS( fp, data )                                               \
{                                                                       \
    char __data_buf[BUFSIZ];                                            \
    fgets(__data_buf, BUFSIZ, fp);                                      \
    strcpy(data,__data_buf);                                            \
}

/*
 * Global data
 */
int nx, ny, nt;			/* Point counts, local for MPI version */
int nxg, nyg;			/* Global point counts */
int *nxs, *nys;			/* # of points in each partition */
double a;			/* Diffusion constant */
double dx, dy, dt;		/* Spatial and time steps */
double *ws_uold, *ws_unew;	/* Pointers to work space */
double **uold, **unew;		/* Solution arrays */
double *xc, *yc;		/* Coordinates */
int snapshot_freq;		/* Output frequency control */
double dx2, dy2;		/* dx2 <- dx * dx, dy2 <- dy * dy */

/*
 * Global functions
 */

/* Input parameters from a file */
void input(void)
{
    FILE *fp = NULL;
    char title[256];

    /* Should always check the returned pointer */
    if (NULL == (fp = fopen("input.dat", "r")))
    {
        perror("Can't open file \"input.dat\"");
        exit(0);
    }

    /* As in Fortran, every READ starts a new line */
    READS(fp, title);
    READN(fp, a);
    READN(fp, nx);
    READN(fp, ny);
    READN(fp, nt);
    READN(fp, dt);
    READN(fp, snapshot_freq);

    // Calculate grid sizes   |<-------- L=1 ------->|
    dx = 1.0 / (nx-1); //  o--*--*- ... ... ... ... -*--o
    dy = 1.0 / (ny-1); //  0  1  2  ... ... ... ...  nx nx+1

    dx2 = dx*dx;
    dy2 = dy*dy;

    /* Display parameters */
    printf("\nInput paramters:\n");
    printf("  a: %f\n", a);
    printf("  nx: %d\n", nx);
    printf("  ny: %d\n", ny);
    printf("  nt: %d\n", nt);
    printf("  Time step: %f\n", dt);
    printf("  Snapshot frequency:  %d\n", snapshot_freq);

    fclose(fp);

    /* Set global point counts */
    nxg = nx;
    nyg = ny;

    return;
}

/* Write 2d output for GNUPLOT */
void output(double t, int istep)
{
    double x, y;
    int i, j;
    FILE *fp;
    char fname[FILENAME_MAX];
    static int icount=0;

#ifndef NO_OUPUT
    if (istep % snapshot_freq != 0) return;

#ifdef USE_MPI
    if (istep > 0) 
    {	    
        /* This can be replaced by MPI_Gather() call */
        if (0 < rank) 
        {
#ifdef BUGGY_SUBARRAY_ACCESS		
            MPI_Send(&unew[1][1], ny*nx, MPI_DOUBLE, 0, 2, comm);
#else
            MPI_Send(&unew[1][0], ny*(nx+2), MPI_DOUBLE, 0, 2, comm);
#endif	    
            return;
        }
        else
        {
            MPI_Status s;
            int p, q;
            int nyl, j_start;

            for (p = 1; p < num_procs; p++)
            {
                MPI_Probe(MPI_ANY_SOURCE, tag, comm, &s);
                q = s.MPI_SOURCE;
                nyl = (nyg + q) / num_procs;
                j_start = global_yindex_start(q,nys) - 1;
#ifdef TRACE		
                printf("Rank: %d, j_start=%d, nyl=%d\n",q,j_start,nyl);
#endif		
#ifdef BUGGY_SUBARRAY_ACCESS		
                MPI_Recv(&unew[j_start+1][1], nyl*nx, MPI_DOUBLE, q, 2, comm, &s);
#else
                MPI_Recv(&unew[j_start+1][0], nyl*(nx+2), MPI_DOUBLE, q, 2, comm, &s);
#endif		
            }
        }
    }
    else
    {
        if (rank > 0) return;
    }	
    /* Only MPI code rank 0 will come to this point */
#endif /* USE_MPI */

    sprintf(fname, "output.%d", icount);
    if (NULL == (fp = fopen(fname, "w")))
    {
        printf("Can't open file %s: %s", fname, strerror(errno));
        exit(0);
    }

    printf("Writing solution at t=%f to file\n", t);
    for (j = 1; j <= nyg; j++)
    {
        y = yc[j];
        for (i = 1; i <= nxg; i++)
        {
            x = xc[i];
            fprintf(fp, "%f %f %f %f\n", t, x, y, unew[j][i]);
        }
        fprintf(fp,"\n");
    }

    icount++;
    fclose(fp);
#endif

    return;
}
