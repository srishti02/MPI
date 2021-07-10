#include "solve.h"

// Get the start index (1-start) of the local segment
int global_yindex_start(int rank, int *ns)
{
#ifndef USE_MPI
    return 0;
#else
    int i, ig_start;

    ig_start = 1;
    if (0 < rank)
    {
        ig_start = 0;
        for (i=0; i<rank; i++)
            ig_start += ns[i];
        ig_start++;
    }

    return ig_start;
#endif /* USE_MPI */
}

// Get the global index (0-start)
int global_yindex(int j, int rank, int *ns) 
{
    // Note j is 0 -start, adjust the conversion to 0-start
#ifndef USE_MPI
    return j;
#else
    return global_yindex_start(rank,ns) -1 + j; 
#endif
}
