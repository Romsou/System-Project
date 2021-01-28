#!/bin/bash

exec=nachos-step6

old_pwd=$(pwd)
num_of_machines=$1

cd ../build

for i in $(seq 0 $(($1 - 1))); do
    set -x
    ( ./${exec} -m $i -o $(($i + 1))) &
    set + x
done

set -x
( ./${exec} -m $num_of_machines -o 0) &
set +x

cd $old_pwd
echo "Type ctrl + c to quit"