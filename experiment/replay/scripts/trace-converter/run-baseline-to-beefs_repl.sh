#!/bin/bash

CONVERTER=../beefs_repl/baseline_capture_to_beefsrepl_input.py
OPERATION_TYPE="rw rr seqw seqr"
ORDER_POLICY="fs"
TRACE_DIR=/home/worker/data/local/experiments/collect
OUTPUT_DIR=/home/worker/data/local/experiments/converted

mkdir -p $OUTPUT_DIR

for op_type in $OPERATION_TYPE; do
	for trace_file in $TRACE_DIR/*.*.$op_type.*.*.*.*.*.*.load.out; do
		trace_file_name=$(basename $trace_file)
		echo "--> converting: $trace_file_name"
		python $CONVERTER $op_type $ORDER_POLICY < $trace_file >$OUTPUT_DIR/$trace_file_name
	done
done
