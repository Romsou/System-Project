#!/usr/bin/env bash



test_threads() {
    echo -e "==== Test des threads ====\n"
    ./nachos-final -f 
    for file in $(ls|grep -i ^thread|grep -v .o$); do
        echo -e "--- Test $file ---\n"
        ./nachos-final -cp $file $file
        ./nachos-final -rs 1 -x $file
    done
}

test_processes() {
    echo -e "==== Test des processus ====\n"
    ./nachos-final -f 
    for file in $(ls|grep -i ^process|grep -v .o$); do
        echo -e "--- Test $file ---\n"
        ./nachos-final -cp $file $file
        ./nachos-final -rs 1 -x $file
    done
}

test_filesystem() {
    echo -e "==== Test du système de fichier ====\n"
    ./nachos-final -f
    for file in $(ls|grep -i ^filesys|grep -v .o$); do
        echo -e "--- Test $file ---\n"
        ./nachos-final -cp $file $file
        ./nachos-final -rs 1 -x $file
    done
}

test_network() {
    echo -e "==== Test du système de fichier ====\n"
    cd $old_pwd
    ./test_network_machine_ring.sh 3
}

help() {
    echo "Usage: $0 [-t] [-p] [-f] [-n] [-h]"
    echo
    echo "-t    Tests threads"
    echo "-p    Tests processes"
    echo "-f    Tests filesystem"
    echo "-n    Tests network"
}

old_pwd=$(pwd)
build_dir=../build

cd $build_dir

echo -n "Compilation... "
make clean > /dev/null ; make > /dev/null
[ $? != 0 ] && echo "Erreur de compilation" && exit 1
echo "Terminée"

while getopts "tpfnh" option; do
    case $option in
        t) test_threads ;;
        p) test_processes ;;
        f) test_filesystem ;;
        n) test_network ;;
        h) help ;;
    esac
done

[ $OPTIND -eq 1 ] && help && exit 1

exit 0