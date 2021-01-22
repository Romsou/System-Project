#include "system.h"

void usage(){
	PutString("Bienvenu dans console de test de systeme de fichier\n");
	PutString("Voici les commandes\n");
	PutString("Pour les fichiers : new name \t del name\n");
	PutString("Pour les repertoires : mkdir name, rmdir name, cd name, ls name\n");
	PutString("Pour quitter : 'q' \n")

}

void FS_console(){
	char *command;

	usage();
	while(TRUE){
		PutChar(">");

		command = GetString();
		if(!strcmp (*command, "new")){

		}else if(){
			
		}
	}

}