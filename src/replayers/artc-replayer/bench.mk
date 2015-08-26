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

BENCH_CC = gcc

ifeq ($(DEBUG),)
	BENCH_COPT = -O2
	BENCH_CDEBUG =
else
	BENCH_COPT = -O0
	BENCH_CDEBUG = -ggdb3
endif

BENCH_CWARN = -Wall -Wno-unused-variable -Wno-deprecated-declarations \
	-Werror=implicit-function-declaration

ifeq ($(shell uname -m),x86_64)
	BENCH_CMODEL = -mcmodel=medium
else ifeq ($(shell uname -m),amd64)
	BENCH_CMODEL = -mcmodel=medium
else
	BENCH_CMODEL =
endif

BENCH_CFLAGS = $(BENCH_CWARN) $(BENCH_COPT) $(BENCH_CDEBUG) $(BENCH_CMODEL) \
	-std=gnu99 -fPIC -fvisibility=hidden

ifneq ($(BENCH_DEBUG),)
	BENCH_CFLAGS += -DTRACE_SYSCALLS
endif

ifeq ($(shell uname -s),Darwin)
	DSO_EXT:=dylib
	DSOFLAGS:=-dynamiclib
else
	DSO_EXT:=so
	DSOFLAGS:=-shared
endif

TYPECHECK_STRUCTS = benchmark,event,resstatus,fd_info,fduse,genaio,aio_info,pathgen,pginfo

ifeq ($(shell which pahole >/dev/null 2>&1 && echo yes),yes)
	getversion = $(shell pahole -q -E -C $(TYPECHECK_STRUCTS) $1 | md5sum | cut -d' ' -f1)
else
	getversion = $(shell echo >&2 "pahole not found, install \`dwarves' to enable versioning")
endif

benchdefault: bench.$(DSO_EXT)

# These can take a while to generate and compile
.PRECIOUS: %.gp.c %.gp.o
%.gp.c: %.gperf
	gperf --output-file=$@ $<

%.o: %.c
	$(BENCH_CC) -c $(BENCH_CFLAGS) -o $@ $<

%.dbg.o: %.c
	$(BENCH_CC) -c $(BENCH_CFLAGS) -ggdb3 -o $@ $<

benchclean:
	rm -f *.o bench.$(DSO_EXT) .stamp
