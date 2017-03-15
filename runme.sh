#!/bin/bash

POWCAPS=(115 110 105 100 95 90 85 80 75 70 65 64)
NESTS=64
CONFS=(A B C D)
echo -e "n_ests conf pow old_time paviz_time"
for C in "${CONFS[@]}"; do
    for P in "${POWCAPS[@]}"; do
        foo=$(echo -e ${P})
        bar=$(./prog -i 1 -p ${P} -n ${NESTS} -c ${C} | grep Runtime | cut -d " " -f 2)
        echo ${NESTS} ${C} ${foo} ${bar}
    done
done

