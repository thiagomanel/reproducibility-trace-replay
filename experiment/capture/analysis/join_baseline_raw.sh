#!/bin/bash
#
# It joins output dat to ease plotting. We do not summarize results
#
# Usage: ./join_data.sh outdir > join.data
#
# outdir follows bellow pattern:
#  outdir
#    |--- machine_name_1
#       |------ vm_type_A
#       |------ vm_type_B
#    |---- machine_name_2
#       |------ vm_type_A
#       |------ vm_type_B

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

# outdir/machine/vm_type/$sample.$random.$workload.$nthreasd.$delay.$nops.$blksize.$sample.$nbackground.$mode.load
# e.g outdir/lambaru/small/9.8804569.seqw.4.10000.1000.4096.9.4.same-disk.load.err
echo -e "tid req begin end offset machine trace vm sample random workload nforeground delay nops blksize nbackground storage"
for file in `find ${outdir}/${factor_dir} -name "*.load.out"`
do
    vm_type=$(basename $(dirname $file))
    machine=$(basename $(dirname $(dirname $file)))
    trace_type="baseline"

    basepath=`basename $file`
    cfg=`echo $basepath | cut -d"." -f1-7,9-10 | sed 's/\./ /g'`

    grep -v debug $file | awk -v args="$machine $trace_type $vm_type $cfg" '{ printf("%s %s %s %s %s %s\n", $1, $2, $3, $4,$5, args); }'
done
