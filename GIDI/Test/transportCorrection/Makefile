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
	if [ ! -e Outputs ]; then mkdir Outputs; fi
	./transportCorrection --tid O16 > Outputs/O16.out
	python ../Utilities/diff.py "transportCorrection/transportCorrection O16" Benchmarks/O16.out Outputs/O16.out
	./transportCorrection --tid Th227 > Outputs/Th227.out
	python ../Utilities/diff.py "transportCorrection/transportCorrection Th227" Benchmarks/Th227.out Outputs/Th227.out
