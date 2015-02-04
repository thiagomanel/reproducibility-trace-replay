#!/bin/bash

nsamples=$1
nprocs=$2
nops=$3

trace_tool="baseline"
#for baseline this option does not apply. anyway, I'll pass it aroung
#to avoid changes in output file name
storage_output_mode="same-disk"

for workload in rr rw seqr seqw
do
  #for nthreads in 1 4
  for nthreads in $nprocs
  do
    #for delay in 0 10000
    for delay in 0
    do
      for nops in $nops
      do
        for blksize in 4096
	do
          for background in 1 $nprocs
	  do
     	    echo $workload $ntreads $delay $nops $blksize $nsamples $background
	    python coordinator.py $workload $nthreads $delay $nops $blksize $nsamples $background $storage_output_mode $trace_tool
          done
        done
      done
    done
  done
done
