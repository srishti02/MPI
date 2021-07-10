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

TARGETS=helloworld-fortran.exe helloworld-c.exe helloworld-cpp.exe

all: jobscripts

clean:
	rm -f *.exe *.out sbatchjob-*.sh run-*.sh

jobscripts: $(TARGETS)
	./gen-scripts.sh

helloworld-fortran.exe: helloworld.f90
	$(MPIFC) $(FCFLAGS) -o $@ $<

helloworld-c.exe: helloworld.c
	$(MPICC) $(CFLAGS) -o $@ $<

helloworld-cpp.exe: helloworld.cpp
	$(MPICXX) $(CXXFLAGS) -o $@ $<
