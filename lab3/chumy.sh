#!/usr/bin/bash

saveconf="$(cat config.conf)"
eval $(sed 's/ //g;s/\[.*\]//g' config.conf)
make $Policy

> output.txt
for block in $(seq 3 6)
do
    for set in $(seq 0 $((6-$block)))
    do
        way=$((6-$set-$block))
        printf "$set $way $block\n"
        #change config and print config
        printf "[cache]\nSet = $((2**$set))\nWay = $((2**$way))\nBlockSize = $((2**$block))\nPolicy = \"fifo\"">config.conf
        #make score
        make test 2>&1 | grep -A 3 ==== >>output.txt
    done
done

saveconf="$(cat config.conf)"