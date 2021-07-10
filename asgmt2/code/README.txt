Assignment 2

WHAT TO DO

Complete the given parallel code for the 2D diffusion problem. You only need 
to complete the two calls MPI_Sendrecv() to accomplish the bidirectional chain 
data movement. Compile and run the complete code with the Makefile and input 
data provided. Then run the plot script to see the solution at different time 
steps.

The code given is for the calculation of u on a subdomain assigned to
a process. The original domain is decomposed in y-direction into N
subdomain, where N is equal to the number of processes. Each subdomain
needs to exchange data on the boundary with its upper and lower neighbors
with ranks labelled "above" and "below" respectively, as illustrated 
shown

                                 
                  |             above             |
                  +-------------------------------+

                  +-------------------------------+
                  |                               |
                  |              me               |
                  |                               |
                  +-------------------------------+

                  +-------------------------------+
                  |             below             |

                  
The rank of the upper and lower neighbors are defined as global variables
in init.c already, as shown below

    above = rank + 1;
    if (above == num_procs) above = MPI_PROC_NULL;
    below = rank - 1;
    if (below < 0) below = MPI_PROC_NULL;

All you need to fill the missing ranks using these two variables in the 
MPI_Sendrecv() calls. That's all!

FILES

All the files are included in this directory.

TESTING

Compile the code using command

    make

Run the command with command

    mpirun -n 4 ./heatc_mpi_sendrecv

Run script

    ./plot

to visualize the the solution. You should have 11 PNG file output.*.png 
generated. Use an image viewer to see the solution at different time steps.
