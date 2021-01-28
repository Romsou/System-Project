#!/bin/bash

exec=nachos-final
old_wd=$(pwd)
green="\e[32m"
red="\e[31m"
reset="\e[0m"
checkmark="\xE2\x9C\x94"

gecho() {
    echo -e ${green}$*${reset}
}

redecho() {
    echo -e ${red}$*${reset}
}

cd ../build

echo -n "Compilation."
make clean > /dev/null && make > /dev/null
echo -n "."
[ ! -f $exec ] && echo "Erreur de compilation"
echo -n ". "
gecho "\xE2\x9C\x94"

gecho "Formattage du disque dur..."
./$exec -f > /dev/null
echo 

files="$(ls|grep -i ^user|grep -v .o$|head -n 4)"

gecho "Copie de tous les fichiers dans le système..."
for i in $files; do
    echo -n "Copie de $i..."
     
    ./$exec -cp $i $i > /dev/null
    gecho " $checkmark"
done

echo
gecho "Test des exécutable..."
for i in $files; do
    echo -n "Lancement de "
    gecho $i
    ./$exec -x $i
    echo 
done

echo "Test réseau simple..."
cd $old_pwd
./test_network_machine_ring.sh

exit 0