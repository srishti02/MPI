// "read_array.c" Read am m-by-n array from a binary file and display the
// the portion of the array of size up to 20 x 12
#include <stdio.h>
#include <stdlib.h>

#ifndef min
#define min(x,y) ((x) < (y)? (x):(y))
#endif

int main(int argc, char* argv[])
{
    int i, j, m, n;
    float *a;
    FILE *fp;

    if (argc != 4) {
        printf("Usage: %s rows cols\n", argv[0]);
        abort();
    }
    fp = fopen(argv[1],"rb");
    m = atoi(argv[2]);
    n = atoi(argv[3]);
    printf("m=%d, n=%d\n", m, n);
    a = (float *)malloc(sizeof(float)*m*n);

    fread(a,sizeof(float),m*n,fp);
    printf("\nA:\n");
    for (int i=0; i<min(m,20); i++)
    {    
        for (int j=0; j<min(n,12); j++)
            printf("%5.1f", a[i*n+j]);
        printf("\n");
    }	
    fclose(fp);

    free(a);
    return 0;
}
