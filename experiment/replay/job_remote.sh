#!/bin/bash

TARGET_MNT="/data"
bin_dir="scripts/"
BEEFS_REPL_BIN=/home/user/scripts/write_bias_check/replay/scripts/beefs_repl/Beefs-trace-replayer/beefs_replayer

function drop_caches {
   local base=$1
   local outpath=$base".dropcache.remote.out"
   local errpath=$base".dropcache.remote.err"
   $bin_dir/drop_cache.sh > $outpath 2> $errpath
}

function xumount {
    umount $TARGET_MNT
}

function xmount {
    mount $TARGET_MNT
}

if [ "$#" -ne 2 ]; then
    echo "Illegal number of parameters. Usage: $0 trace_input base_out"
    exit 1
fi

#we assume we will receive fullpath within vm
trace_input=$1
base_out=$2

echo "Umount vm target fs"
xumount
echo "Drop vm page cache"
drop_caches $base_out
echo "Mount vm target fs"
xmount
sleep 5

timing="faster"
num_workers=1
add_delay_us=0
delay=0

echo "Start load" $BEEFS_REPL_BIN $trace_input $timing $num_workers $add_delay_us "debug"
$BEEFS_REPL_BIN $trace_input $timing $num_workers $add_delay_us "debug" > $base_out".out" 2> $base_out".err"
