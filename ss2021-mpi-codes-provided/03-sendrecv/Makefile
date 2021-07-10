#
# To build programs using Intel tools...
#   module load StdEnv/2020 intel/2021.2.0 openmpi/4.1.1
#   make clean TOOLCHAIN=intel
#   make TOOLCHAIN=intel
#
# To build programs using GCC and OpenMPI...
#   module load StdEnv/2020 gcc/10.2.0 openmpi/4.0.5
#   make clean TOOLCHAIN=gcc
#   make TOOLCHAIN=gcc
#
MPICC=mpicc
MPICXX=mpic++
MPIFC=mpifort

ifeq ($(TOOLCHAIN),intel)
# Use Intel compilers...
COMMON_FLAGS=-O3 -march=broadwell
CFLAGS=-Wall $(COMMON_FLAGS)
CXXFLAGS=-std=c++98 -Wall $(COMMON_FLAGS)
FCFLAGS=-stand f18 $(COMMON_FLAGS)
else ifeq ($(TOOLCHAIN),gcc)
# Use GCC...
COMMON_FLAGS=-Wall -Wextra -O3 -march=broadwell
CFLAGS=$(COMMON_FLAGS)
CXXFLAGS=-std=c++98 $(COMMON_FLAGS)
FCFLAGS=-std=f2018 $(COMMON_FLAGS)
else
$(error TOOLCHAIN must be set to intel or gcc)
endif

TARGETS=sendrecv-fortran.exe sendrecv-c.exe sendrecv-cpp.exe

all: jobscripts

clean:
	rm -f *.exe *.out sbatchjob-*.sh run-*.sh

jobscripts: $(TARGETS)
	./gen-scripts.sh

sendrecv-fortran.exe: sendrecv.f90
	$(MPIFC) $(FCFLAGS) -o $@ $<

sendrecv-c.exe: sendrecv.c
	$(MPICC) $(CFLAGS) -o $@ $<

sendrecv-cpp.exe: sendrecv.cpp
	$(MPICXX) $(CXXFLAGS) -o $@ $<
