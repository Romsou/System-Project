#include "system.h"

#define SIZE 10

static split_string(char *command, char **split){
	char delim = ' ';
	char *cpy;

	int debut = 0;
	int index = 0;
	int diff = 0;
	
	int size = strlen(command);
	strcpy(cpy,command);
	
	for(int i = 0; i < size ; i++){
		if(command[i]=='\n' || command[i]=='\0')
			break;
		if(command[i] == delim){
			diff = i - debut;
			strcpy(split[index],command+debut,diff);
			debut = i+1;
		}
	}
}

void usage(){
	PutString("Bienvenu dans console de test de systeme de fichier\n");
	PutString("Voici les commandes\n");
	PutString("Pour les fichiers : new name, del name, cat name\n");
	PutString("Pour les repertoires : mkdir name, rmdir name, cd name, ls name\n");
	PutString("Pour quitter : 'q' \n")
}

void FS_console(){
	char *command;
	char **split = malloc(sizeof(char)*SIZE*2);


	usage();
	while(TRUE){
		PutChar(">");

		command = GetString();
		split_string(command,split);

		if(!strcmp (split[0], "new")){

		}else if(!strcmp (split[0], "new")){

		}else if(!strcmp (split[0], "del")){

		}else if(!strcmp (split[0], "cat")){

		}else if(!strcmp (split[0], "mkdir")){

		}else if(!strcmp (split[0], "rmdir")){

		}else if(!strcmp (split[0], "cd")){

		}else if(!strcmp (split[0], "ls")){

		}else if(!strcmp (split[0], "quit") || !strcmp (split[0], "q") ){
			PutString("Fin de la console");
			break;
		}else{
			usage();
		}
	}

}