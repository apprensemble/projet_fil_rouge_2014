/************************************************************************
*
* Projet: Projet du master IDL – 2014
*
*************************************************************************
*
* Camille Di Pietro - Thierry Alaux - Michel Sérieys
*
*************************************************************************/

#include "ser-connexion.h"


int setConnexion(char *nomProg, char *port)
{
    socklen_t size_addrcli;


    int r;                          /* Temporary return code. */

    /* Initialisation de la liste chainée contenant les noms des fichiers disponibler */
    ma_liste = NULL;
    ma_liste = create_element("Quitter",ma_liste);		// Ajout du premier élément en position 0 = "Quitter"
    ls_rep(ma_liste, TXT_CATALOG_PATH);	// Initialise la liste chainée avec les fichiers du répertoire txt de catalogue



    /* Choix de l'action effectuée par un processus à la réception d'un signal spécifique */
    sigactionManager();			/* Signal handler specification structure */



    /* 2. Search for information related to the server **************************/
    /* The structure is emptied so that only useful fields are filled.*/
    memset(&info_request, 0, sizeof(struct addrinfo));
    /* AF_UNSPEC allows IPv4 and IPv6 addresses. */
    info_request.ai_family = AF_UNSPEC;
    /* SOCK_DGRAM -> UDP, SOCK_STREAM -> TCP */
    info_request.ai_socktype = SOCK_STREAM;
    /* For wildcard IP address */
    info_request.ai_flags = AI_PASSIVE;
    /* Any protocol */
    info_request.ai_protocol = 0;
    info_request.ai_canonname = NULL;
    info_request.ai_addr = NULL;
    info_request.ai_next = NULL;


    /* Call getaddrinfo with no host (NULL), the port, info_request and
    	 obtain answers about the server in a list of structures addrinfo
    	 starting at p_info_server. */

    if( (r=getaddrinfo(NULL, port, &info_request, &p_info_server)) != 0)
    {
        /* If not null, the return value r gives an error code, printed
        	 as a human readable message using gai_strerror(). */
        printf("[%d] %s: failed while calling getaddrinfo(): %s.\n",getpid() ,
               nomProg, gai_strerror(r));
        exit(EXIT_FAILURE);
    }


    printf("[%d] Recupération des infos ok. Tentative d'ouverture de la socket.\n",getpid());
    fflush(stdout);

    /* 3. Look at a valid socket for connecting to the server *******************/
    /* Scrutinize the list of struct addrinfo, and try each address by
    	 creating the listening socket and binding it.*/


    for (p_addrinfo = p_info_server; p_addrinfo != NULL; p_addrinfo = p_addrinfo->ai_next)
    {

        /* If socket() failed, then try the next structure in the list. */
        /* NB: we could also use socket(AF_INET,SOCK_STREAM,0) */
        if( (sockser=socket(p_addrinfo->ai_family,
                            p_addrinfo->ai_socktype,
                            p_addrinfo->ai_protocol)) == -1 )
            continue;

        /* Else try bind(). If success, then stop scrutinizing the list:
        	 a valid socket has been found. */
        if (bind(sockser, p_addrinfo->ai_addr, p_addrinfo->ai_addrlen) == 0)
            break;

        /* Else close the socket that failed with connect() and try the next
        	 structure in the list. */
        close(sockser);
    }


    if (p_addrinfo == NULL)
    {
        printf("[%d] Failed to bind.\n",getpid());
        exit(EXIT_FAILURE);
    }


    /* Now the list of addrinfo structures can be deleted. */
    freeaddrinfo(p_info_server);

    printf("[%d] Socket ok. Attente de connexion.\n",getpid());
    fflush(stdout);




    /* 4. Waiting for connection ************************************************/
    if( listen(sockser, LISTEN_BACKLOG) == -1 )
    {
        perror("main()/listen()\n");
        exit(EXIT_FAILURE);
    }

    size_addrcli = sizeof(struct sockaddr_in);
    while (true)
    {
        if ( (sockcli=accept(sockser, (struct sockaddr *) &addrcli, &size_addrcli)) == -1 )
        {
            perror("main()/accept()\n");
            exit(EXIT_FAILURE);
        }
        pid_t p_pid_fils = NULL;

        switch ( p_pid_fils = fork() )
        {
        case (pid_t)-1:
            /* Echec du fork(), par exemple manque de mémoire... */
            printf("[%d] Echec du fork(), par exemple manque de mémoire...\n",getpid());
            perror("main/fork\n");
            close(sockcli);
            close(sockser);
            return EXIT_FAILURE;

        case (pid_t)0:
            /* Retour du fork() = 0 donc c'est le processus fils */
            close(sockser);
            sonProcess(sockcli, (struct sockaddr *) &addrcli, size_addrcli, ma_liste);
            printf("[%d] .............. Fin de ce processus fils.\n", getpid());
            close(sockcli);
            exit(EXIT_SUCCESS);

        default:
            /* Retour du fork() != 0 donc c'est le pere */
            printf("[%d] Création du processus fils %d.\n",getpid(), p_pid_fils);
            printf("[%d] Attente d'une nouvelle connexion client. \n",getpid());
            fflush(stdout);
        }
    }

    free_list(ma_liste);	/* Libération de la mémoire utilisé par la liste chainée contenant le menu */

    return EXIT_SUCCESS;

}




void sigactionManager()
{
    sig_ser.sa_handler=&sigHandler;	/* HANDLER 							                            */
    /* Masque complet 											                                        */
    /* sigfillset() remplit totalement l'ensemble de signaux set en incluant tout les signaux 		    */
    if( sigemptyset( &sig_ser.sa_mask ) != 0) 	/* Renvoie 0 si réussi sinon -1				            */
    {
        perror("pere/sigfillset");
        exit(EXIT_FAILURE);
    }
    sig_ser.sa_flags = SA_RESTART;

    /* Mise en place du gestionnaire de signaux sur SIGCHLD					                            */
    if( sigaction(SIGCHLD, &sig_ser, NULL) != 0)
    {
        perror("pere/sigaction");
        exit(EXIT_FAILURE);
    }
}



void sigHandler( int signal)
{
    pid_t pid_fils;
    int status;
    while( (pid_fils = waitpid(-1,&status,WNOHANG)) != 0)
    {
        if(pid_fils == -1)
        {
            if ( errno == ECHILD ) return;
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
        printf("[%d] Processus \"%d\" terminé avec signal %d. Code retour: %d.\n", getpid(), pid_fils,signal, WEXITSTATUS(status));
        fflush(stdout);




        // WIFEXITED :renvoie vrai si le statut provient d'un processus fils qui s'est terminé normalement.
        // WEXITSTATUS : renvoie le code de retour du processus fils
        if (WIFEXITED(status))
        {

        // WIFSIGNALED : renvoie vrai si le statut provient d'un processus fils qui s'est terminé à cause de la réception d'un signal
        // WTERMSIG : renvoie la valeur du signal qui a provoqué la terminaison du processus fils.
        }
        else if (WIFSIGNALED(status))
        {
            printf("[%d] Tué par le signal %d.\n", getpid(), WTERMSIG(status));

        // WIFSTOPPED : renvoie vrai si le fils a été arrêté par la délivrance d'un signal.
        // WSTOPSIG : renvoie le numéro du signal qui a causé l'arrêt du fils.
        }
        else if (WIFSTOPPED(status))
        {
            printf("[%d] Arrêté par le signal %d.\n", getpid(), WSTOPSIG(status));

        //WIFCONTINUED : renvoie vrai si le processus fils a été relancé par la délivrance du signal SIGCONT.
        }
        else if (WIFCONTINUED(status))
        {
            printf("[%d] Relancé.\n", getpid());
        }

    }
}


