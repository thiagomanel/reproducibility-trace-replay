#!/bin/bash

OUTDIR="/local/manel/doc/metrology/collect/"
bin_dir="scripts/"
remote_dir="/home/user/scripts/write_bias_check/replay/"

function drop_caches {
   local base=$1
   local outpath=$base".dropcache.local.out"
   local errpath=$base".dropcache.local.err"
   ./$bin_dir/drop_cache.sh > $outpath 2> $errpath
}

if [ "$#" -ne 2 ]; then
    echo "Illegal number of parameters. Usage: $0 replay_input nsamples"
     exit 1
fi

replay_input=$1
nsamples=$2

remote_base_out_dir="/tmp/"
for sample in `seq $nsamples`
do
   echo "Running sample: " $sample
   base_out=$sample.$RANDOM.`basename $replay_input`".beefs_repl"
   remote_base_out=$remote_base_out_dir/$base_out
   local_base_out=$OUTDIR/$base_out

   echo "Drop local vm page cache"
   drop_caches $local_base_out

   echo "Start load"
   sleep 5
   ssh -p 2222 root@localhost $remote_dir/job_remote.sh $remote_dir/data/$replay_input $remote_base_out
   sleep 5
   rsync --remove-source-files -ave "ssh -p 2222" root@localhost:/tmp/* $OUTDIR
done
