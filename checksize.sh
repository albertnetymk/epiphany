#!/bin/bash
s=$(size -A $1 | awk '{ if ( NF>2 && $3 > 0 && $3 < 65535  ) sum+=$2   } END { print sum   }' | xargs printf "0x%x\n")
if (( $s > 0xffff )); then
    echo "$1" has larger footprint than sram
    false
else
    echo "$1" is ok
fi
