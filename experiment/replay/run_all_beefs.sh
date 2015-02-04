#!/bin/bash

inputlist=$1
workload=$2

for f in `grep $workload $inputlist`
do
   n_repl_threads=`echo $f | cut -d"." -f4`
   python coordinator.py event $f conservative $n_repl_threads 15
   python coordinator.py event $f faster $n_repl_threads 15
done
