SHELL = /bin/ksh

# <<BEGIN-copyright>>
# Copyright 2019, Lawrence Livermore National Security, LLC.
# See the top-level COPYRIGHT file for details.
# 
# SPDX-License-Identifier: MIT
# <<END-copyright>>

.PHONY: default include lib bin docs check clean realclean doDIRS

DIRS = Src include lib Test bin Doc

GIDI_PLUS_PATH ?= $(abspath ..)
export GIDI_PLUS_PATH

default: include lib
	$(MAKE) bin

include:
	cd include; $(MAKE)

lib:
	cd lib; $(MAKE)

bin:
	cd bin; $(MAKE) bin

check:
	cd Test; $(MAKE) check

clean:
	$(MAKE) doDIRS _DIRS="$(DIRS)" _TARGET=clean

realclean:
	$(MAKE) doDIRS _DIRS="$(DIRS)" _TARGET=realclean

docs:
	doxygen Doc/doxygen/Doxyfile

doDIRS:
	SAVED_PWD=`pwd`; \
	for DIR in $(_DIRS); do cd $$DIR; $(MAKE) $(_TARGET); cd $$SAVED_PWD; done
