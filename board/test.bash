#!/bin/bash

# count=4
# for i in $(seq 1 3); do
#     for j in $(seq 0 3); do
#         cp -n core0/coordinates.c core$count/coordinates.c
#         let count++
#     done
# done
count=0
for i in $(seq 0 3); do
    for j in $(seq 0 3); do
        row=$(( 32 + $i ))
        col=$(( 36 + $j ))
        echo $count, $row, $col
        row_script="s!ROW_,[0-9]{2}!ROW_,$row!"
        col_script="s!COL_,[0-9]{2}!COL_,$col!"
        sed -r -i $row_script core$count/coordinates.c
        sed -r -i $col_script core$count/coordinates.c
        # cat core$count/coordinates.c
        let count++
    done
done
