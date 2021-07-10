#
# To build programs using Intel tools...
#   module load StdEnv/2020 intel/2021.2.0 openmpi/4.1.1 tbb
#   make clean TOOLCHAIN=intel
#   make TOOLCHAIN=intel
#
# To build programs using GCC and OpenMPI...
#   module load StdEnv/2020 gcc/10.2.0 openmpi/4.0.5 tbb
#   make clean TOOLCHAIN=gcc
#   make TOOLCHAIN=gcc
#
MPICC=mpicc
MPICXX=mpic++
MPIFC=mpifort

INCLDIR=../include

ifeq ($(TOOLCHAIN),intel)
# Use Intel compilers...
COMMON_FLAGS=-O3 -march=broadwell
CFLAGS=-std=c99 -Wall $(COMMON_FLAGS) -fopenmp
CXXFLAGS=-std=c++17 -I$(INCLDIR) -Wall $(COMMON_FLAGS) -pthread
CXX_LDFLAGS=-pthread -ltbb
FCFLAGS=-stand f18 $(COMMON_FLAGS) -fopenmp
else ifeq ($(TOOLCHAIN),gcc)
# Use GCC...
COMMON_FLAGS=-Wall -Wextra -O3 -march=broadwell
CFLAGS=-std=c99 $(COMMON_FLAGS) -fopenmp
CXXFLAGS=-std=c++17 -I$(INCLDIR) $(COMMON_FLAGS) -pthread
CXX_LDFLAGS=-pthread -ltbb
FCFLAGS=-std=f2018 $(COMMON_FLAGS) -fopenmp
else
$(error TOOLCHAIN must be set to intel or gcc)
endif


TARGETS=pi-hybrid-fortran.exe pi-hybrid-c.exe pi-hybrid-cpp.exe

all: jobscripts

clean:
	rm -f *.exe *.out sbatchjob-*.sh run-*.sh

jobscripts: $(TARGETS)
	./gen-scripts.sh

pi-hybrid-fortran.exe: pi-hybrid.f90
	$(MPIFC) $(FCFLAGS) -o $@ $<

pi-hybrid-c.exe: pi-hybrid.c
	$(MPICC) $(CFLAGS) -o $@ $<

pi-hybrid-cpp.exe: pi-hybrid.cpp \
	$(INCLDIR)/iota_iterator.hpp \
	$(INCLDIR)/numerically_integrate.hpp \
	$(INCLDIR)/paralg_tbb_control.hpp
	$(MPICXX) $(CXXFLAGS) -o $@ $< $(CXX_LDFLAGS)
