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

dataroot="./ibench"

progname=`basename "$0"`
scriptdir=`dirname "$0"`

benchset=(`make list`)

artrun="${ARTRUN:-../artrun}"

usage()
{
	echo "Usage: $progname [-n NUM] [-s STARTNUM] TAG [ARGS...]"
}

numruns=1
startnum=0

while getopts ":n:s:" opt; do
	case "$opt" in
	n) numruns="$OPTARG" ;;
	s) startnum="$OPTARG" ;;
	*) usage >&2; exit 1 ;;
	esac
done

endnum=$((startnum+numruns))

shift $((OPTIND-1))
tag="$1"
shift

tmp=("$@")
argv=()
for ((i=0; i < ${#tmp[@]} - 1; i++)); do
	argv=("${argv[@]}" "${tmp[i]}")
done
dir="${tmp[i]}"

for b in "${benchset[@]}"; do
	confirmed=false
	for ((i=startnum; i<endnum; i++)); do
		for f in "$dataroot/$b/data/$tag-$i."{out,err}; do
			if [ -e "$f" ]; then
				echo >&2 "'$f' exists."
				read >&2 -p "Continue, overwriting existing data? [y/N] " resp
				if [ "${resp:0:1}" == "y" ]; then
					confirmed=true
					break
				else
					echo >&2 "Cancelled."
					exit 1
				fi
			fi
		done
		$confirmed && break
	done
	$confirmed && break
done

if [ "`uname -s`" == "Darwin" ]; then
	dsoext=dylib
else
	dsoext=so
fi

for b in "${benchset[@]}"; do
	bench="./ibench/$b/$b.$dsoext"
	datadir="$dataroot/$b/data"
	mkdir -p "$datadir"

	for ((i=startnum; i<endnum; i++)); do
		err="$datadir/$tag-$i.err"
		out="$datadir/$tag-$i.out"
		echo "$b[$i]: running benchmark..."
		time "$artrun" "${argv[@]}" "$bench" "$dir" 2> "$err" > "$out"
	done
done
