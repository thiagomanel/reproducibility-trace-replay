#
# This file is part of artc, the Approximate-Replay Trace Compiler.
#
# Copyright (C) 2012, 2013 Zev Weiss <zev@cs.wisc.edu>
#
# artc is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, either version 2 of the License, or (at your
# option) any later version.
#
# artc is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with artc.  If not, see <http://www.gnu.org/licenses/>.
#

# Disable goofball default rules
.SUFFIXES:

CC = gcc
BISON = bison
FLEX = flex
ARTC ?= ./artc

BASE_CFLAGS = -fdiagnostics-show-option -Wall -Wno-format-zero-length -std=gnu99

ifeq ($(DEBUG),)
	COPT = -O2
	CDEBUG =
else
	COPT = -O0
	CDEBUG = -ggdb3
endif

RTLIBS:=-lpthread
ifeq ($(shell uname -s),Linux)
	RTLIBS += -lrt -ldl -laio
endif

ifeq ($(shell uname -s),Darwin)
	DSO_EXT:=dylib
else
	DSO_EXT:=so
endif

CFLAGS = $(BASE_CFLAGS) $(COPT) $(CDEBUG) -DARTC

# gcc outputs multiple lines separated with '\' at the end of each
# continued line, so here we join them back together.
joinlines = sed -e ':again' -e '/\\$$/{' -e 'N; b again' -e '}' -e 's/\\\n//g'

benchdeps = $(eval $(shell $(BENCH_CC) $(BENCH_CFLAGS) -MM -MG $2 $1 | $(joinlines)))
_deps = $(eval $(shell $(CC) $(CFLAGS) -MM -MG $2 $1 | $(joinlines)))
deps = $(call _deps,$1)
ldeps = $(call _deps,$1,-MT $(1:.l=.yy.o) -x c)
ydeps = $(call _deps,$1,-MT $(1:.y=.tab.o) -x c)

# These aren't slow to generate or anything, but make removing them
# automatically can lead to spurious rebuilds (at least with the way
# the rest of this is set up).
.PRECIOUS: %.tab.h %.tab.c %.yy.h %.yy.c

%.tab.h: %.tab.c
	@touch $@

%.tab.c: %.y
	$(BISON) -Wall --defines=$*.tab.h -o $@ $<

%.yy.h: %.yy.c
	@touch $@

%.yy.c: %.l
	$(FLEX) --header-file=$*.yy.h -o $@ $<

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

.PRECIOUS: %.bench.d/.stamp
%.bench.d/.stamp: %.trace %.init $(ARTC)
	$(ARTC) $(ARTC_FLAGS) --ibench $< $*.init $*.bench.d
	@touch $@

%.bench.d/.stamp: %.strace %.init %.iostat $(ARTC)
	$(ARTC) $(ARTC_FLAGS) --strace --iostat=$*.iostat $< $*.init $*.bench.d
	@touch $@

%.bench.d/.stamp: %.strace %.init $(ARTC)
	$(ARTC) $(ARTC_FLAGS) --strace $< $*.init $*.bench.d
	@touch $@

%.dot: %.strace %.init $(ARTC)
	$(ARTC) $(ARTC_FLAGS) --strace --extract=depgraph $*.strace $*.init > $@

%.$(DSO_EXT): %.bench.d/.stamp
	$(MAKE) -C $*.bench.d bench.$(DSO_EXT)
	mv $*.bench.d/bench.$(DSO_EXT) $@
