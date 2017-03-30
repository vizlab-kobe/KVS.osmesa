#!/bin/bash

src_dir=..
exe_file=RayCasting
job_name=kvs_osmesa_test

## Functions
useage_exit()
{
    cat << EOS 1>&2
[auto.sh] Useage: $0 [Options]
Options:
        -h, --help
        -t, --thread_num number_of_threads
EOS
    exit 1
}

num_judge()
{
    if [ -z $1 ]; then
	return 1
    fi
    expr $1 + 1 > /dev/null 2>&1
    ret=$?
    if [ $ret -lt 2 ]; then
	return 0
    else
	return 1
    fi
}

make_job_script()
{
    cat << EOS > job.sh
#!/bin/bash

#PJM -N "$job_name"
#PJM -L "rscgrp=small"
#PJM -L "node=$node_num"
#PJM -L "elapse=10:00"
#PJM -j
export OMP_NUM_THREADS=$thread_num
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/local/osmesa/lib

./$exe_file

EOS
}

## Main part
node_num=1
thread_num=1

# Option analysis
for opt in "$@"
do
    case "$opt" in
	'-h'|'--help' )
	    useage_exit
	    ;;
	'-t'|'--thread_num' )
	    num_judge $2
	    if [ $? == 1 ]; then
		echo "[auto.sh] option requires an argument : $1" 1>&2
		useage_exit
	    fi
	    thread_num=$2
	    shift 2
	    ;;
	-* )
	    echo "[auto.sh] illegal option" 1>&2
	    useage_exit
	    ;;
    esac
done

cd ${0%/*}
dir_name=${job_name}_$(date +%Y-%m-%d_%H-%M-%S)
mkdir $dir_name
cp ${src_dir}/${exe_file} $dir_name
cd $dir_name
make_job_script
pjsub job.sh
