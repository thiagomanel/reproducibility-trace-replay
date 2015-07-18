#!/bin/bash

source ../../conf/experiment.conf

CONVERTER=./convert-baseline-to-beefs_repl.py
WORKLOAD="rw rr seqw seqr"
ORDER_POLICY="fs"

mkdir -p $CONVERTED_OUTDIR

for workload in $WORKLOAD; do
	for trace_file in $CAPTURE_OUTDIR/*.*.$workload.*.*.*.*.*.*.load.out; do
		trace_file_name=$(basename $trace_file)
		echo "--> converting: $trace_file_name"
		python $CONVERTER $op_type $ORDER_POLICY < $trace_file >$CONVERTED_OUTDIR/$trace_file_name
	done
done
