################################################################################
#
# Flags for libraries, etc.
#
################################################################################

CC              = mpicc
FC              = f90
FFLAGS          += -ffortran-bounds-check
CFLAGS          += -g \
		-DUSE_MPI \
		#-DBUGGY_SUBARRAY_ACCESS \
		#-DTRACE \
		#-DUSE_NONBLOCK_MPI_CALLS \
		#-DNO_OUTPUT # -Wall \
		#-DUSE_SENDRECV \
FLDFLAGS        += -lm
CLDFLAGS        += -lm

MAKEFILE        = Makefile
