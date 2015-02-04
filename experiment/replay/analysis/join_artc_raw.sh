#!/bin/bash
#
# It joins output data from replay metrology.
# Usage: ./join_data.sh outdir > join.data

if [ $# -ne 1 ]
then
    echo "Usage:" $0 "outdir"
    exit 1
fi

outdir=$1

if [ ! -d $outdir ]
then
    echo "directory" $outdir "not found"
    exit 1
fi

# $capture_source/$machine/$vm/$replay_sample.$replay_random.$replayer_type.$sample.$random.$workload.$nthreasd.$delay.$nops.$blksize.$sample.$nbackground.$mode.load.workload.$timing_policy.$ntreads.$sample
# e.g baseline/enchada/small/0.1007072.compile.7.3887476.seqw.4.10000.1000.4096.7.4.same-disk.load.out.artc.workload-target.conservative.1.0.replay.out
echo -e "tid call begin end response capture_src machine vm replay_sample replay_rand replayer sample rand workload nforeground delay nops blksize nbackground storage t_policy"

for file in `find ${outdir} -name "*.replay.out"`
do
    vm_type=$(basename $(dirname $file))
    capture_src=$(basename $(dirname $(dirname $file)))
    machine=$(basename $(dirname $(dirname $(dirname $file))))

    basepath=`basename $file`
    cfg=`echo $basepath | cut -d"." -f1-10,12-13,17 | sed 's/\./ /g'`

    #we select only the data operation 4096 4096 is the byte count
    python filter_artc.py < $file | awk -v args="$capture_src $machine $vm_type $cfg" '{printf("%s %s %s %s %s %s\n", $1, $2, $3, $4, $5, args);}'
done
