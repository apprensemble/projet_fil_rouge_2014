#ifndef CLI_CONNEXION_H_INCLUDED
#define CLI_CONNEXION_H_INCLUDED

#include <netdb.h>		/* définitions des opérations de base de données en réseau */
#include <stdio.h>      /* printf...*/
#include <stdbool.h> 	/* false, true ...*/
#include <stdlib.h>     /* EXIT_FAILURE...*/
#include <sys/stat.h>   /* mkdir */
#include <unistd.h>   /* getpid()... */
#include <stdio.h>      /* printf...*/
  
#include <lib-cat.h> /* Shared Lib from catalogue */

#define BUF_SIZE 500



/* ouvre une connexion vers le serveur */
int sockConnect(char* serverAddress,char* serverPort);
int sendFilename(char* filename);
int getFileContent(char* progName, char* filename, int sockcli);
int setSvgFilename(char* filename, char* nomFichierSvg);
int searchServerInfo(char* progName, char* serverAddress, char* serverPort);
int searchServerInfo();
list getFilenameList(int ockcli);

list str2List(char * str);





#endif // CLI_CONNEXION_H_INCLUDED

