#Rapport - Etape 2
## Partie 4 - Appel systeme `PutChar`
###Action 4
`SynchConsole *synchconsole` déclarée en variable gloable, initilialsée dans `Initialize` et détruit dans `Cleanup`
Ajout de l'import `include "synchconsole.h"` dans system.h et compilé avec USER_FLAVORS=step2
Testé avec `make clean ; make ; ./nachos-step2 -x putchar`
###Action 6
Dans le fichier `test/putchar.c`, fontion `Halt()` commentée : une exception est déclenchée après l'exécution de la fonction `print` de type 1 (-> SyscallException) avec comme code 1. Après l'execution du test, un exception par défaut est levé `handleError()`
Une solution est de remplacer `handleError()` par `handleHalt()` comme fonction par défaut.