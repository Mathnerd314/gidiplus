SHELL = /bin/ksh

# <<BEGIN-copyright>>
# Copyright 2019, Lawrence Livermore National Security, LLC.
# See the top-level COPYRIGHT file for details.
# 
# SPDX-License-Identifier: MIT
# <<END-copyright>>

GIDI_PLUS_PATH ?= $(abspath ../../..)
cppSource = $(sort $(wildcard *.cpp))

include $(GIDI_PLUS_PATH)/Makefile.paths
include ../Makefile.check

check: upscatterModelB
	./upscatterModelB > o
	../Utilities/diff.com upscatterModelB/upscatterModelB upscatterModelB.out o
