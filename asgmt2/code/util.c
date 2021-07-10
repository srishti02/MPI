/*
 * Utility routines
 * $Log: util.c,v $
 */

/* Swap 2d arrays a and b */
void aswap(double ***a, double ***b)
{
    double** tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;

    return;
}
