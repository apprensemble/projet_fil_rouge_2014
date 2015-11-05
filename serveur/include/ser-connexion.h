#ifndef SER_CONNEXION_H_INCLUDED
#define SER_CONNEXION_H_INCLUDED


#include <sys/wait.h>   /* waitpid()... */
#include <stdbool.h> 	/* false, true ...*/
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>		/* sigaction, sigemptyset, struct sigaction...*/
#include <string.h>
#include <unistd.h>   /* getpid()... */
#include <errno.h>	/* ECHILD... */
#include <lib-cat.h> /* Shared Lib from catalogue */

#include "ser-son.h"

#define LISTEN_BACKLOG 50
#define TXT_CATALOG_PATH "../../catalogue/txt/"




struct addrinfo info_request;   /* Used to request infos about the server */
struct addrinfo *p_info_server; /* Contain infos about the server as a list
																		 of struct addrinfo. */
struct addrinfo *p_addrinfo;    /* Used to scrutinize the list returned by
																		 getaddrinfo. */ 

int sockser;                   /* Socket descriptor for waiting connection. */
int sockcli;                   /* Socket created by a connection. */

struct sockaddr_in addrcli;

struct sigaction sig_ser;		    /* Déclaration d'une structure pour gestionnaire de signaux 	*/

list ma_liste;	/* Déclaration de la liste chainée contenant les noms des fichiers disponibler */




void sigactionManager();
void sigHandler( int signal);
int sockBind(char* nomProg);
int setConnexion(char* nomProg, char *port);

#endif // SER_CONNEXION_H_INCLUDED
