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
MPIFC=mpif90

ifeq ($(TOOLCHAIN),intel)
# Use Intel compilers...
COMMON_FLAGS=-O3 -march=broadwell
CFLAGS=-std=c99 -Wall $(COMMON_FLAGS)
CXXFLAGS=-std=c++11 -Wall $(COMMON_FLAGS)
FCFLAGS=-stand f18 $(COMMON_FLAGS)
else ifeq ($(TOOLCHAIN),gcc)
# Use GCC...
COMMON_FLAGS=-Wall -Wextra -O3 -march=broadwell
CFLAGS=-std=c99 $(COMMON_FLAGS)
CXXFLAGS=-std=c++11 $(COMMON_FLAGS)
FCFLAGS=-std=f2018 $(COMMON_FLAGS)
else
$(error TOOLCHAIN must be set to intel or gcc)
endif

TARGETS=pi-fortran.exe pi-c.exe pi-cpp.exe

all: jobscripts

clean:
	rm -f *.exe *.out sbatchjob-*.sh run-*.sh

jobscripts: $(TARGETS)
	./gen-scripts.sh

pi-fortran.exe: pi.f90
	$(MPIFC) $(FCFLAGS) -o $@ $<

pi-c.exe: pi.c
	$(MPICC) $(CFLAGS) -o $@ $<

pi-cpp.exe: pi.cpp
	$(MPICXX) $(CXXFLAGS) -o $@ $<
