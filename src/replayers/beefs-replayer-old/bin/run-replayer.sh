#!/bin/bash

source ../../conf/experiment.conf

TIMING_POLICY="faster conservative"
WORKLOAD="rr rw seqr seqw"
N_THREADS="1 2 4 8"
DELAYS="0 50"
N_SAMPLES="10"

mkdir -p $REPLAYER_OUTDIR

counter=0

for workload in $WORKLOAD; do
	for nthreads in $N_THREAD; do
		for trace in $CONVERTED_OUTDIR/*.*.$workload.$nthreads.*.*.load.out; do
			for timing_policy in $TIMING_POLICY; do
				for sample in `seq 1 $N_SAMPLES`; do
					echo $((++counter))
					output_file=$REPLAYER_OUTDIR/$(basename $trace).$sample.$timing_policy

					echo "--> executing: $BEEFS_REPLAYER $trace $timing_policy $nthreads 0 debug > $output_file.out 2> $output_file.err"

					echo "umounting /data..."
					umount $TARGET_MNT

					echo "mounting /data..."
					mount $TARGET_MNT

					$CHECK_PATH > $output_file.check.out 2> $output_file.check.err
					$DROP_CACHE_PATH > $output_file.drop_cache.out 2> $output_file.drop_cache.err
					$BEEFS_REPLAYER $trace $timing_policy $nthreads 0 debug > $output_file.out 2> $output_file.err
				done
			done
		done
	done
done
