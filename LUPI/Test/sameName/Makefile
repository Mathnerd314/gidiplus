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
	rm -rf Outputs/*
	touch Outputs/sameName.out
	touch Outputs/sameName.err
	for executable in $?; do ./sameName.com ./$$executable; done
	-diff -r Benchmarks Outputs > /dev/null; if [ $$? -ne 0 ]; then echo "FAILURE: sameName"; fi
