/************************************************************************
*
Projet: Projet du master IDL – 2014
*
*************************************************************************
*
* Camille Di Pietro - Thierry Alaux - Michel Sérieys
*
*************************************************************************/

#include <stdio.h>      /* printf...*/
#include <stdlib.h>     /* EXIT_FAILURE...*/

#include "cli-connexion.h"


int
main(int argc, char **argv)
{
    /* 1. Check arguments *******************************************************/
    if (argc < 3)
    {
        printf("[\n%d] Usage: %s host port\n", getpid(), argv[0] );
        exit(EXIT_FAILURE);
    }
    printf("\n[%d] Arguments de la ligne de commande ok. Tentative de connexion au serveur \"%s:%d\".\n" ,
           getpid(), argv[1], atoi(argv[2]));
    fflush(stdout);



    /* 2. Search for information related to the server **************************/
    searchServerInfo(argv[0], argv[1], argv[2]);

    exit(EXIT_SUCCESS);
}









