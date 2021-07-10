// "mpi_pio_hello.c" - A simple MPI code that demonstrates all processes
// write a message to the same disk file in predefine order.

#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char* argv[])
{
    int ierr, rank, size;
    MPI_Offset offset;
    MPI_File fh;
    MPI_Status status;
    const int msgsize=6; 
    char msg[msgsize+1]; // Set enough to hold "hello " and "world!";)
    char filename[132] = "helloworld.txt";

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Choose the output file name, default is helloworld.txt
    if (0 == rank)
        if (2 == argc)
            strcpy(filename,argv[1]); // Note: strlen() exludes '\0'
    MPI_Bcast(filename,strlen(filename)+1,MPI_CHAR,0,MPI_COMM_WORLD);

    // Prepre the message for this process
    if ((rank % 2) == 0) 
        strcpy (msg, "Hello ");
    else 
        strcpy (msg, "World!");

    // Calculate the offset from where the message is written
    offset = msgsize*rank;

    // Every process opens file and writes, then closes it
    MPI_File_open(MPI_COMM_WORLD, filename, 
        MPI_MODE_CREATE|MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
    MPI_File_seek(fh, offset, MPI_SEEK_SET);
    MPI_File_write(fh, msg, msgsize, MPI_CHAR, &status);
    MPI_File_close(&fh);

    // Shutdown the MPI environment
    MPI_Finalize();

    return 0;
}
