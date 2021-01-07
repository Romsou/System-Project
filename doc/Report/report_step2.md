#Rapport - Etape 2
## Partie 4 - Appel systeme `PutChar`
###Action 4
`SynchConsole *synchconsole` déclarée en variable gloable, initilialsée dans `Initialize` et détruit dans `Cleanup`
Ajout de l'import `include "synchconsole.h"` dans system.h et compilé avec USER_FLAVORS=step2
Testé avec `make clean ; make ; ./nachos-step2 -x putchar`
###Action 6
Dans le fichier `test/putchar.c`, fontion `Halt()` commentée : une exception est déclenchée après l'exécution de la fonction `print` de type 1 (-> SyscallException) avec comme code 1. Après l'execution du test, un exception par défaut est levé `handleError()`
Solution 1 est de remplacer `handleError()` par `handleHalt()` comme fonction par défaut.
Solution 2 le faire appeler par __start dans start.S à la place de Exit. `start.S:28: jal   Halt` plutôt que `jal Exit`
<strong>Un peu de doc :</strong>
Une fois la méthode `main` exécutée, il y a un appel système à `Exit` avec 0 comme argument. Cet appel n'est pas obligatoire, il est provoqué sciemment par le système. L'execution du `main()` utilisateur est provoqué par l'appel __start du fichier `test/start.S` (le fichier qui fait le lien entre l'espace utilisateur et le noyau) - le code de cet appel est situé à l'adresse 0, c'est pour cela qu'il débute la séquence d'éxécution. C'est aussi cet appel qui appel Exit, un autre appel systeme défini dans le même fichier.

Pour avoir une fonction de sortie qui permet de lire et renvoyer les données on crée appel système End qui va traiter cette partie et pourra si besoin envoyer un message au processus fils etc.