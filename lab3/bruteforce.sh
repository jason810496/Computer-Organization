#!/usr/bin/bash

saveconf="$(cat config.conf)"
eval $(sed 's/ //g;s/\[.*\]//g' config.conf)
make $Policy
echo "Policy: "$Policy

# init variables
bestMissRate=999
bestSet=0
bestWay=0
bestBlock=0

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
        curMissRate=$(tail -n 1 output.txt | cut -d ' ' -f 3)

        #update score
        if [ $(echo "$curMissRate < $bestMissRate" | bc) -eq 1 ]
        then
            bestMissRate=$curMissRate
            bestSet=$set
            bestWay=$way
            bestBlock=$block
        fi
    done
done

echo "=======================================================================" >> output.txt
echo "The best data cache setting is:" >> output.txt
echo "Setting: Set = $((2**$bestSet)), Way = $((2**$bestWay)), BlockSize = $((2**$bestBlock))" >> output.txt
echo "Miss Rate: "$bestMissRate >> output.txt


saveconf="$(cat config.conf)"
