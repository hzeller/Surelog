#!/bin/bash
SURELOG=$(realpath $(dirname $0))/build/bin/surelog
PROFILER_LIB=/usr/lib/x86_64-linux-gnu/libprofiler.so

USE_PERF=1

if [ $# -ne 1 ]; then
    echo "Usage: $0 <sl-basename-somewhere-below-.>"
    echo "Example: $0 Opentitan"
    echo "--- Here are few that might work ---"
    (for f in $(find . -name "*.sl"); do
         if cat $f | grep -- "-parse" > /dev/null; then
             echo $(basename -s.sl $f)
         fi
     done) | sort | uniq -u | column
    exit 1
fi

TEST=$1

#----------------
CPU_PROFILE_FILE=/tmp/$TEST.pprof
SL_FILE=$(find . -name "$TEST.sl")

if [ -z "$SL_FILE" ]; then
    echo "Couldn't find any $TEST.sl"
    exit 1
fi

cd $(dirname $SL_FILE)

rm -rf slpp_all/

if [ $USE_PERF -eq 1 ] ; then
  perf record -g $SURELOG -f $TEST.sl
  echo "In directory $(pwd)"
  perf report -g 'graph,0.5,caller' -Mintel
  echo "---------------------"
  echo "$(realpath $(pwd))"
  echo "---------------------"
else
  time bash -c "LD_PRELOAD=$PROFILER_LIB CPUPROFILE=$CPU_PROFILE_FILE $SURELOG -f $TEST.sl"
  pprof $SURELOG $CPU_PROFILE_FILE
fi
