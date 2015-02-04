#!/bin/bash
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

traceurl="https://research.cs.wisc.edu/adsl/Software/artc/ibench.tar.bz2"

if which curl &>/dev/null; then
	dl() { curl "$@"; }
elif which wget &>/dev/null; then
	dl() { wget -O - "$@"; }
else
	echo >&2 "`basename $0`curl or wget required, neither found."
	exit 1
fi

dl "$traceurl" | tar xjf -
