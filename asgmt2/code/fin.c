/*
 * Copyright (C) 2006 Ge Baolai <gebaolai@gmail.com>
 * $Log: init.f90,v $
 */

#ifdef USE_MPI
#include <mpi.h>
#endif /* USE_MPI */

/* Finalize the environment */
void fin(void)
{
#ifdef USE_MPI
    MPI_Finalize();
#endif
    return;
}
