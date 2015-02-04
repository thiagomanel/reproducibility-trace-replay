#!/bin/bash

trace_tool=$1
trace_input=$2
nthreads=$3
nsamples=$4

#FIXME
timing="conservative"

python coordinator.py $trace_tool $trace_input $timing $nthreads $nsamples
