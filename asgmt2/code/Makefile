################################################################################
#
# Makefile for 2D diffusion equation
# Copyright(C) 2006-2018 Western University
# SHARCNET | Compute Canada - Calcul Canada
# Ge Baolai <bge@sharcnet.ca>
#
################################################################################

# Read compiler flags from the include file, if existent
-include ./Makefile.inc

# Object files
C_COMMON_O = \
	main.o \
	io.o \
	init.o \
	fin.o \
	ic.o \
	bc.o \
	util.o \
	index.o 

C_O = solve.o $(C_COMMON_O)
C_MPI_SENDRECV_O = solve_mpi_sendrecv.o $(C_COMMON_O)

# Rules
default: all
all: heatc heatc_mpi_sendrecv

%.o : %.c 
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c -o $@ $<

heatc: $(C_O)
	$(CC) -o heatc $(C_O) $(CLDFLAGS)

heatc_mpi_sendrecv: $(C_MPI_SENDRECV_O)
	$(CC) -o heatc_mpi_sendrecv $(C_MPI_SENDRECV_O) $(CLDFLAGS)

clean: cleanf cleanc

cleanf:
cleanc:
	rm -f heatc heatc_mpi heatc_mpi_sendrecv heatc_mpi_nonblock
	rm -f *.o core.* j*k 
	rm -f output.*
	rm -f output*.png
	rm -f *.o 

