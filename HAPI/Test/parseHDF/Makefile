SHELL = /bin/ksh

# <<BEGIN-copyright>>
# Copyright 2019, Lawrence Livermore National Security, LLC.
# See the top-level COPYRIGHT file for details.
# 
# SPDX-License-Identifier: MIT
# <<END-copyright>>

GIDI_PLUS_PATH ?= $(abspath ../../..)
CppSource = $(sort $(wildcard *.cpp))

include $(GIDI_PLUS_PATH)/Makefile.paths
include ../Makefile.check

check: $(Executables)

ifdef HAPI_USE_HDF5
check: parseHDF
	if [ ! -e Outputs ]; then mkdir Outputs; fi
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(HDF5_LIB) ./parseHDF > Outputs/parseHDF.out
	../Utilities/diff.com parseHDF/parseHDF ../purePugi/Benchmarks/parseHDF.out Outputs/parseHDF.out
else
check: parseHDF
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(HDF5_LIB) ./parseHDF
endif
