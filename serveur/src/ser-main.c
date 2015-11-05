/************************************************************************
*
* Projet: Projet du master IDL – 2014
*
*************************************************************************
*
* Camille Di Pietro - Thierry Alaux - Michel Sérieys
*
*************************************************************************/

#include <unistd.h>   /* getpid()... */
#include <stdio.h>      /* printf...*/
#include <stdlib.h>     /* EXIT_FAILURE...*/

#include "ser-connexion.h"
//#include <lib-cat/lib-cat.h> /* Shared Lib from catalogue */


int main(int argc, char *argv[]) {


     if( argc != 2 ) {
		printf("\n Usage: %s port\n", argv[0]);
		exit(EXIT_FAILURE);
	}


	printf("\n[%d] Arguments de la ligne de commande ok.\n",getpid());
	fflush(stdout);

	setConnexion(argv[0], argv[1]);

    return EXIT_SUCCESS;

}
