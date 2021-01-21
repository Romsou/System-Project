#!/bin/bash

nachos=nachos-step4

execute_programs() {
    for program in $(ls | grep -i ^process | cut -d "." -f1 | sort | uniq); do
        echo
        echo -e "==== \e[32mExecuting $program\e[0m ====\n"
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
