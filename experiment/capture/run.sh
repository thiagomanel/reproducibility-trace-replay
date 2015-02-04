#!/bin/bash

nsamples=$1
nops=5000
storage_output_mode="same-disk"

#for workload in seqr seqw rr rw
for workload in seqr seqw
do
  #for nthreads in 1 2 3 4 5 6 7 8 9
  for nthreads in 16 32 64
  do
    for delay in 0
    do
      for blksize in 4096
      do
        for background in 0
	do
	  #for trace_tool in strace stap
	  for trace_tool in strace
	  do
     	    echo $workload $ntreads $delay $nops $blksize $nsamples $background
	    python coordinator.py $workload $nthreads $delay $nops $blksize $nsamples $background $storage_output_mode $trace_tool
	  done
        done
      done
    done
  done
done
