#!/bin/bash

# Bash script to test the ring topology
# in nachOS.
#
# Takes the first parameter as the number of machines to
# run, then, makes the last one send a message to 0.

exec=nachos-step6

old_pwd=$(pwd)
num_of_machines=$1

help() {
    echo "Usage:"
    echo -e "\t./test_network_machine_ring <number of machines>"

}

main() {
    cd ../build

    for i in $(seq 0 $(($1 - 1))); do
        set -x
        (./${exec} -m $i -o $(($i + 1))) &
        set +x
    done

    set -x
    (./${exec} -m $num_of_machines -o 0) &
    set +x

    cd $old_pwd
    echo "Type ctrl + c to quit"
}

if [ $1 == "-h" ]; then
    help
else
    main $1
fi