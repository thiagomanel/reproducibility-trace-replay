#!/bin/bash

#
# This script must be sourced by other, it just has a funciontion that iterates
# over all paramter (workload, number of threads etc)
#
#

WORKLOAD="rr rw seqr seqw"
#N_THREAD="1 2 3 4"
N_THREAD="1 2 4 8"
DELAYS="0 50"
#DELAYS="0"
#N_OPS="5000"
N_OPS="100"
BLKSIZE="4096"
N_SAMPLES=10
#N_BACKGROUND="0 1"
N_BACKGROUND="0"

len() {
	local _array=( $@ )
	echo ${#_array[@]}
}

calculate_total_executions() {
	local s_workload=$(len $WORKLOAD)
	local s_nthreads=$(len $N_THREAD)
	local s_delays=$(len $DELAYS)
	local s_nops=$(len $N_OPS)
	local s_blksize=$(len $BLKSIZE)
	local s_nbackground=$(len $N_BACKGROUND)

  echo "$N_SAMPLES * $s_nbackground * $s_workload * $s_nthreads * $s_delays * $s_nops * $s_blksize" | bc
}

# This is the real thing, if you need to change the behaviour we iterate, this is the place.
exec_iteration() {
	local counter=0;
	local total_executions=$(calculate_total_executions);

	for workload in $WORKLOAD; do
		for nthreads in $N_THREAD; do
			for delay in $DELAYS; do
				for nops in $N_OPS; do
					for blksize in $BLKSIZE; do
						for sample in `seq 1 $N_SAMPLES`; do
							for nbackground in $N_BACKGROUND; do
								counter=$(( ++counter ))
								echo "--> ($counter/$total_executions): workload=$workload, nthreads=$nthreads, delay=$delay, nops=$nops, blksize=$blksize, sample=$sample, nbackground=$nbackground"

								# Here we execute the function given as paramter `$1`
								$1 $workload $nthreads $delay $nops $blksize $sample $nbackground;
							done
						done
					done
				done
			done
		done
	done
}
