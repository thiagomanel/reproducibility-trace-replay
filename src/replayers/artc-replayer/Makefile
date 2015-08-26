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

default: artc artrun

include bench.mk

include common.mk

XXD ?= xxd

%_mk.c: %.mk
	$(XXD) -i $< > $@

%_h.c: %.h
	$(XXD) -i $< > $@

%.1.gz: %.md
	pandoc -s -t man $< | gzip -c > $@

$(call benchdeps,artrun.c)
artrun.o:
	$(BENCH_CC) -c $(BENCH_CFLAGS) -o $@ $<

$(call benchdeps,replay.c)
replay.o:
	$(BENCH_CC) -c $(BENCH_CFLAGS) -o $@ $<

# This is a special case of a *.c file being compiled into a
# differently-named *.o file (the runtime version of it).
$(call benchdeps,event.c,-MT rt_event.o)
rt_event.o:
	$(BENCH_CC) -c $(BENCH_CFLAGS) -o $@ $<

$(call benchdeps,artrun.c,-MT artrun.dbg.o)
version.c: artrun.dbg.o
	@echo "const char version_string[] = \"$(call getversion,$<)\";" > $@

version.o: version.c
	$(BENCH_CC) -c $(BENCH_CFLAGS) -o $@ $<

artrun: artrun.o rt_event.o replay.o version.o
	$(BENCH_CC) -o $@ $(BENCH_CFLAGS) $^ $(RTLIBS)

ARTC_CSRCS = artc.c fsmodel.c htab.c event.c codegen.c
ARTC_LSRCS = initlex.l ibench-lex.l strace-lex.l iostat-lex.l ibench-initlex.l
ARTC_YSRCS = initparse.y ibench-parse.y strace-parse.y iostat-parse.y ibench-initparse.y

$(foreach src,$(ARTC_CSRCS),$(call deps,$(src)))
$(foreach src,$(ARTC_LSRCS),$(call ldeps,$(src)))
$(foreach src,$(ARTC_YSRCS),$(call ydeps,$(src)))

GENOBJS = trace_h.o bench_mk.o fsmodel_h.o flags_h.o system_h.o misc_h.o
YYOBJS = $(ARTC_YSRCS:.y=.tab.o) $(ARTC_LSRCS:.l=.yy.o)

artc: $(ARTC_CSRCS:.c=.o) $(YYOBJS) $(GENOBJS)
	$(CC) -o $@ $(CFLAGS) $^

clearcache: clearcache.c
	$(CC) -o $@ $(CFLAGS) $<

MANCMDS = artc artrun strace-artc geninit
MANPAGES = $(foreach cmd,$(MANCMDS),$(cmd).1.gz)

docs: $(MANPAGES)

install-clearcache: clearcache
	@[ -n "$(CLEARCACHE_INSTALL_DIR)" ] || { echo >&2 "Must define CLEARCACHE_INSTALL_DIR"; exit 1; }
	install -p -m 4755 -o0 -g0 $< $(CLEARCACHE_INSTALL_DIR)

.PHONY: rtclean
rtclean:
	rm -f artrun artrun.o rt_event.o replay.o version.o version.c

.PHONY: manclean
manclean:
	rm -f $(MANPAGES)

.PHONY: clean
clean:
	rm -f *.yy.[ch] *.tab.[ch] artc artrun clearcache *.o *.a $(GENOBJS:.o=.c) $(MANPAGES)
