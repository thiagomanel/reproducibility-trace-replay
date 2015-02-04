#!/bin/bash

inputlist=$1
workload=$2

for f in `grep $workload $inputlist`
do
   python coordinator.py compile $f root 1 15
done
