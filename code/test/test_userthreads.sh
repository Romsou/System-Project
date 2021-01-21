#!/bin/bash

# Quick integration test (not automated) for user threads


nachos=nachos-step4

execute_programs() {
    for program in $(ls|grep -i ^userthread); do
        echo "executing $program"
        ./${nachos} -x ${program}
        echo "Press any key to start next program"
        read
    done
}

main() {
    old_place=$(pwd)

    cd ../build
    make clean && make && execute_programs
    cd $old_place
}

main

exit 0