#!/usr/bin/env bash

<<<<<<< HEAD
green="\e[32m"
reset="\e[0m"
checkmark="\xE2\x9C\x94"

gecho() {
    echo -e ${green}$*${reset}
}

=======
>>>>>>> 7998c91f3416deb6a479ac76d7ec1504220e2846
test_threads() {
    echo -e "==== Test des threads ====\n"
    ./nachos-final -f >/dev/null
    for file in $(ls | grep -i ^thread | grep -v .o$); do
        echo -e "--- Test $file ---\n"
        ./nachos-final -cp $file $file
        ./nachos-final -rs 1 -x $file
    done
}

test_processes() {
    echo -e "==== Test des processus ====\n"
    ./nachos-step4 -f >/dev/null
    for file in $(ls | grep -i ^process | grep -v .o$); do
        echo -e "--- Test $file ---\n"
        ./nachos-step4 -rs 1 -x $file
    done
}

test_filesystem() {
    echo -e "==== Test du système de fichier ====\n"
    for file in $(ls | grep -i ^filesys | grep -v .o$ | grep -v limit$); do
        echo -e "--- Test \e[32m$file\e[0m ---\n"
        ./nachos-final -f >/dev/null
        ./nachos-final -cp $file $file
        ./nachos-final -rs 1 -x $file
        ./nachos-final -D
    done

    file="filesys_limit"

    echo -e "--- Test \e[32m$file\e[0m ---\n"
    ./nachos-final -f >/dev/null
    ./nachos-final -cp $file $file
    ./nachos-final -rs 1 -x $file
    echo -e "\e[32mContenu de la mémoire\e[0m"
    ./nachos-final -D
}

test_network() {
    echo -e "==== Test du système de fichier ====\n"
    cd $old_pwd
    ./test_network_machine_ring.sh 3
}

help() {
    echo "Usage: $0 [-c] [-t] [-p] [-f] [-n] [-h]"
    echo
    echo "-c    Compiles nachOS. Use BEFORE other parameters if you need it"
    echo "-t    Tests threads"
    echo "-p    Tests processes"
    echo "-f    Tests filesystem"
    echo "-n    Tests network"
}

compile() {
    echo -n "Compilation... "
    make clean >/dev/null
    make >/dev/null
    [ $? != 0 ] && echo "Erreur de compilation" && exit 1
    echo "Terminée"

}

old_pwd=$(pwd)
build_dir=../build

cd $build_dir

while getopts "ctpfnh" option; do
    case $option in
    c) compile ;;
    t) test_threads ;;
    p) test_processes ;;
    f) test_filesystem ;;
    n) test_network ;;
    h) help ;;
    esac
done

[ $OPTIND -eq 1 ] && help && exit 1

exit 0
