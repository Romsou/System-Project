
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
	printf("Pour les fichiers : new name, del name, cat name\n");
	printf("Pour les repertoires : mkdir name, rmdir name, cd name, ls name\n");
	printf("Pour quitter : 'q' \n");
}

void FS_console(){
	char **split = (char **)malloc(sizeof(char)*SIZE*2);
	split[0] = (char *)malloc(sizeof(char)*SIZE);
	split[1] = (char *)malloc(sizeof(char)*SIZE);


	usage();
	while(TRUE){
		printf(">");

		scanf("%s", split[0]);
		
		

		if(strcmp (split[0], "new")==0){
			scanf("%s", split[1]);
			fileSystem->Create(split[1],10);

		}else if(strcmp (split[0], "del")==0){
			scanf("%s", split[1]);
			fileSystem->Remove(split[1]);

		}else if(strcmp (split[0], "cat")==0){
			scanf("%s", split[1]);


		}else if(strcmp (split[0], "mkdir")==0){
			scanf("%s", split[1]);
			fileSystem->CreateDir(split[1]);

		}else if(strcmp (split[0], "rmdir")==0){
			scanf("%s", split[1]);
			fileSystem->RemoveDir(split[1]);

		}else if(strcmp (split[0], "cd")==0){
			scanf("%s", split[1]);
			fileSystem->ChangeDir(split[1]);

		}else if(strcmp (split[0], "ls")==0){
			fileSystem->List();

		}else if(strcmp (split[0], "quit")==0 || strcmp (split[0], "q")==0 ){
			puts("Fin de la console");
			break;
		}else if(strcmp (split[0], "help")==0 || strcmp (split[0], "h")==0 ){
			usage();
		}else{

		}
	}

	free(split[1]);
	free(split[0]);
	free(split);

	return;

}