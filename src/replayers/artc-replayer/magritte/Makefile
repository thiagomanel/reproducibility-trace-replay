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

ARTC = ../artc

include ../common.mk

TRACES = iphoto_start400 iphoto_import400 iphoto_duplicate400 \
	iphoto_edit400 iphoto_delete400 iphoto_view400 itunes_startsmall1 \
	itunes_importsmall1 itunes_importmovie1 itunes_album1 itunes_movie1 \
	imovie_start1 imovie_import1 imovie_add1 imovie_export1 pages_start15 \
	pages_create15 pages_createphoto15 pages_open15 pages_pdf15 \
	pages_pdfphoto15 pages_doc15 pages_docphoto15 numbers_start5 \
	numbers_createcol5 numbers_open5 numbers_xls5 keynote_start20 \
	keynote_create20 keynote_createphoto20 keynote_play20 \
	keynote_playphoto20 keynote_ppt20 keynote_pptphoto20

TRACEDIR = ibench

BENCHBINS = $(foreach t, $(TRACES), $(TRACEDIR)/$t/$t.$(DSO_EXT))
STAMPS = $(foreach t, $(TRACES), $(TRACEDIR)/$t/$t.bench.d/.stamp)

default: benchmarks

.PHONY: stamps
stamps: $(STAMPS)

.PHONY: benchmarks
benchmarks: $(BENCHBINS)

.PHONY: list
list:
	@echo $(TRACES)

.PHONY: cleanall
cleanall:
	for t in $(TRACES); do make -C $(TRACEDIR)/$$t/$$t.bench.d benchclean; done
