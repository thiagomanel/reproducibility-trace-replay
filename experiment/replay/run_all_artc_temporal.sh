#!/bin/bash

inputlist=$1
workload=$2

for f in `grep $workload $inputlist`
do
   python coordinator_temporal.py compile $f temporal 1 15
done
