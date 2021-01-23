
#include "copyright.h"

#include "utility.h"
#include "filesys.h"
#include "system.h"
#include "thread.h"
#include "disk.h"
#include "stats.h"

#include <string.h>

#define SIZE 10



void usage(){
	printf("Bienvenu dans console de test de systeme de fichier\n");
	printf("Voici les commandes\n");
	printf("Pour les fichiers : new name, del name\n");
	printf("Pour les repertoires : mkdir name, rmdir name, cd name, ls\n");
	printf("Pour quitter : 'q' \n");
}

void FS_console(){
	char *command = (char *)malloc(sizeof(char)*SIZE);
	char *name = (char *)malloc(sizeof(char)*SIZE);


	usage();
	while(TRUE){
		printf(">");

		scanf("%s", command);
		
		

		if(strcmp (command, "new")==0){
			scanf("%s", name);
			fileSystem->Create(name,10);

		}else if(strcmp (command, "del")==0){
			scanf("%s", name);
			fileSystem->Remove(name);

		}else if(strcmp (command, "mkdir")==0){
			scanf("%s", name);
			fileSystem->CreateDir(name);

		}else if(strcmp (command, "rmdir")==0){
			scanf("%s", name);
			fileSystem->RemoveDir(name);

		}else if(strcmp (command, "cd")==0){
			scanf("%s", name);
			fileSystem->ChangeDir(name);

		}else if(strcmp (command, "ls")==0){
			fileSystem->List();

		}else if(strcmp (command, "path")==0){


		}else if(strcmp (command, "quit")==0 || strcmp (command, "q")==0 ){
			puts("Fin de la console");
			break;
		}else if(strcmp (command, "help")==0 || strcmp (command, "h")==0 ){
			usage();
		}else{

		}
	}

	free(name);
	free(command);

	return;

}