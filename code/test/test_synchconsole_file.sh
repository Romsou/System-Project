#!/bin/bash

# Test script for synchconsole.cc
#
# This simple script creates two files and
# test whether synchconsole may write in it

exec=nachos-step4

main() {
    echo "Test synchConsole.cc" > ../build/in.txt
    touch ../build/out.txt
    [ ! -f ../build/${exec} ] && make clean && make
    cd ../build
    ./${exec} -sc in.txt out.txt
    
    result=$(diff in.txt out.txt)
    echo $result
    if [ -z $result ]; then
        echo -e "\e[32m\xE2\x9C\x94\e[0m synchConsole has written correctly in out.txt from in.txt"
    else
        echo -e "\e[31m\xE2\x9D\x8C\e[0m synchConsole did not write properly in out.txt from in.txt"
    fi

    rm in.txt out.txt
    cd ../test
    exit 0
}

main
