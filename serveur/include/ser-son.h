#ifndef SER_FILS_H_INCLUDED
#define SER_FILS_H_INCLUDED

#include <sys/wait.h>   /* ... */
#include <unistd.h>   /* getpid()... */
#include <stdlib.h>     /* EXIT_FAILURE...*/
#include <stdio.h>      /* printf...*/
#include <sys/fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>		/* définitions des opérations de base de données en réseau */
#include <errno.h>


#include <lib-cat.h> /* Shared Lib from catalogue */

#define BUF_SIZE 500
#define TXT_CATALOG_PATH "../../catalogue/txt/"


int sonProcess(int sockcli,struct sockaddr* addrcli, socklen_t size_addrcli, list ma_liste);
void sendFilenameList(int sockcli,list ma_liste);
void sendFileContent( int socket, FILE* fd, char* filename);
FILE* openFile(int sockcli, char* filename);



#endif // SER_FILS_H_INCLUDED
