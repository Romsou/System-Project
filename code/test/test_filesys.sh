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



files="filesys_lock filesys_limit" #filesys_create filesys_directory" #

gecho "Tests du systeme de fichier"
echo
for i in $files; do
	gecho "Formattage du disque dur..."
	./$exec -f > /dev/null
	echo -n "Copie de $i..."
     
    ./$exec -cp $i $i > /dev/null
    gecho " $checkmark"

    echo -n "Lancement de "
    gecho $i
    ./$exec -x $i 
    gecho "Contenu de la memoire"
    ./$exec -D
    echo
done
