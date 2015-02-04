#!/bin/bash

nsamples=$1

trace_tool="baseline"
#for baseline this option does not apply. anyway, I'll pass it aroung
#to avoid changes in output file name
storage_output_mode="same-disk"

#for workload in seqr seqw rr rw
for workload in seqr seqw
do
  #for nthreads in 5 6 7 8 9
  for nthreads in 16 32 64
  do
    #for delay in 25 50
    for delay in 0
    do
      for nops in 5000
      do
        for blksize in 4096
	do
          #for background in 0 1 4
          for background in 0
	  do
     	    echo $workload $ntreads $delay $nops $blksize $nsamples $background
	    python coordinator.py $workload $nthreads $delay $nops $blksize $nsamples $background $storage_output_mode $trace_tool
          done
        done
      done
    done
  done
done
