#!/bin/bash

#FIXME paths
artcrun="/local/manel/doc/trace_replay/synt_app/artc/artrun"
target_dir="/local/manel/doc/trace_replay/synt_app/micro/"

#FIXME: output patterns
#ROOT policy
$artcrun --predelay-multiplier=0 $workload/bench.so $target_dir 1> $outdir.artc.root.out 2> $outdir/artc.root.err

#temporal policy
$artcrun --predelay-multiplier=0 --disable-constraints=all --enable-constraints=temporal $workload/bench.so $target_dir 1> $outdir/artc.temporal.out 2> $outdir/artc.temporal.err

#single thread policy
$artcrun --predelay-multiplier=0 --single-thread $workload/bench.so $target_dir 1> $outdir/artc.single.out 2> $outdir/.artc.single.err
