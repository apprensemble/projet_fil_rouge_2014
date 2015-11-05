/************************************************************************
*
Projet: Projet du master IDL – 2014
*
*************************************************************************
*
* Camille Di Pietro - Thierry Alaux - Michel Sérieys
*
*************************************************************************/

#include "cli-connexion.h"


struct addrinfo info_request;   /* Used to request infos about the server */
struct addrinfo *p_info_server; /* Contain infos about the server as a list of struct addrinfo. */
struct addrinfo *p_addrinfo;    /* Used to scrutinize the list returned by getaddrinfo. */

int sockcli;                    /* Socket descriptor. */

ssize_t nread;                  /* Nb bytes read on socket. */
char buf[BUF_SIZE];             /* Buffer for storing bytes read on socket. */



int
searchServerInfo(char* progName, char* serverAddress, char*  serverPort)
{
	
	


	/* Nom du fichier demandé */
    char* filename;

   	/* Déclaration de la liste chainée contenant les noms des fichiers disponibles*/
	list ma_liste = NULL;
	


	/* Set connection with server */
	sockcli = sockConnect(serverAddress, serverPort);



	/* Get list of available files from server */
	ma_liste = getFilenameList(sockcli);
	
	
	/* Boucle infinie */	
	while (true) {
	
		/* Print list of available files */
		printf("\n\n	 Fichiers disponibles\n\n");
		print_list(ma_liste);
		printf("\n");
		
		
		/* Select one file from the list */
		filename = menu_choice(ma_liste);
		
		
		/* Send the requested file to the server. */
		sendFilename(filename);

		
		/* Receive the file content from the server. */
		getFileContent(progName, filename, sockcli);
		
	}
	
	printf("\n[%ld] Fermeture de la socket.\n\n \n", (long int) getpid());
	close(sockcli);
	
    return EXIT_SUCCESS;
}




int sockConnect(char* serverAddress,char* serverPort)
{
	int r;                          /* Temporary return code. */


   /* 2. Search for information related to the server **************************/
    /* The structure is emptied so that only useful fields are filled.*/
    memset(&info_request, 0, sizeof(struct addrinfo));
    info_request.ai_family = AF_UNSPEC;	/* AF_UNSPEC allows IPv4 and IPv6 addresses. */
    info_request.ai_socktype = SOCK_STREAM;	/* SOCK_DGRAM -> UDP, SOCK_STREAM -> TCP */
    info_request.ai_flags = 0;
    info_request.ai_protocol = 0;	/* Any protocol. */

    /* Call getaddrinfo with the server host, server port, info_request and
    	 obtain answers about the server in a list of structures addrinfo
    	 starting at p_info_server. */
//    if ( (r=getaddrinfo(argv[1], argv[2], &info_request, &p_info_server)) != 0)
    if ( (r=getaddrinfo(serverAddress, serverPort, &info_request, &p_info_server)) != 0)
    {
        /* If not null, the return value r gives an error code, printed
        	 as a human readable message using gai_strerror(). */
        printf("[%d] Echec de l'appel de getaddrinfo(): %s.\n",
               getpid(), gai_strerror(r));
        exit(EXIT_FAILURE);
    }

    printf("[%d] Récupération des infos fournies par le serveur réussi.\n", getpid());
    fflush(stdout);



    /* 3. Look at a valid socket for connecting to the server *******************/
    /* Scrutinize the list of struct addrinfo, and try each address by
    	 creating the connection socket and connecting to the server.*/
    for (p_addrinfo = p_info_server; p_addrinfo != NULL; p_addrinfo = p_addrinfo->ai_next)
    {

        /* If socket() failed, then try the next structure in the list. */
        if ( (sockcli = socket(p_addrinfo->ai_family,
                               p_addrinfo->ai_socktype,
                               p_addrinfo->ai_protocol)) == -1)
            continue;

        /* Else try connect(). If success, then stop scrutinizing the list:
        	 a valid socket has been found. */
        if (connect(sockcli, p_addrinfo->ai_addr, p_addrinfo->ai_addrlen) != -1)
            break;

        /* Else close the socket that failed with connect() and try the next
        	 structure in the list. */
        close(sockcli);
    }

    /* If p_addrinfo is null, the list scrutinizing terminated without finding
    	 a valid socket. */
    	 
     if (p_addrinfo == NULL)
    {
        printf("[%d] Failed to connect to %s:%d.\n", getpid(), serverAddress, atoi(serverPort) );
        exit(EXIT_FAILURE);
    }

    /* Now the list of addrinfo structures can be deleted. */
    freeaddrinfo(p_info_server);

    printf("[%d] Ouverture de la socket réussie.\n", getpid());
    fflush(stdout);
    
    
    
    return sockcli;
}


int sendFilename(char* filename)
{
    /* 5. Send a message to the server. *****************************************/

    //getFilename (argv[3],filename);

    //filename=buf;
    fflush(stdout);
    //  Envoie le nom du fichier y compris l'octet 0 de la fin.
    if ( write(sockcli, filename, strlen(filename)+1) != strlen(filename)+1)
    {
        perror("main()/send()");
        fflush(stderr);
        printf("[%d] Echec à l'ouverture du socket.\n", getpid());
        fflush(stdout);
        exit(EXIT_FAILURE);
    }


    printf("[%d] Fichier demandé: %s.\n", getpid(), filename);
    fflush(stdout);
 return EXIT_SUCCESS;

}


int getFileContent(char* progName,char* filename,int sockcli)
{
	
    char nomFichierSvg[50];	// Nom du fichier de sauvegarde du résultat de la requète
    
    setSvgFilename(filename, nomFichierSvg);
    

    
    
    mkdir("../tmp",S_IRWXU | S_IRWXG |S_IRWXO);	// création du répertoire tmp s'il n'existe pas
    

    printf("[%d] Récupère le fichier et l'écrit dans la sortie standard. \n\n", getpid());

    FILE * fd;
    fd = fopen(nomFichierSvg , "w");
    while(true)
    {
        memset(buf, 0, BUF_SIZE);

        // version avec recv
//        if ( (nread = recv(sockcli, buf, BUF_SIZE, 0)) == -1 )
        if ( (nread = read(sockcli, buf, BUF_SIZE)) == -1 )	// Lit le socket
        {
            perror(" main()/read()\n");
            fflush(stderr);
            printf("[%ld] %s: Echec de lecture sur la socket.\n",(long int) getpid(), progName);
            fflush(stdout);
            exit(EXIT_FAILURE);
        }
        
        
        fputs( buf, fd);		// Ecrit dans le fichier
        printf("%s",buf);	// Ecrit dans la sortie standard
        fflush(stdout);
        if ( nread < BUF_SIZE )
        {

            printf("\n[%ld] Le transfert du fichier \"%s\" est terminé (voir %s). \n", (long int) getpid(), filename, nomFichierSvg);

            fflush(fd);
            fflush(stdout);
            

            if (ferror(fd))
            {
                /* une erreur s'est produite */
                perror (filename);
            }

            if(fclose(fd) == EOF)	// ferme le fichier
            {
                printf("[%d] .............. Problème de fermeture du fichier \"%s\".\n",getpid() , filename);
                exit(1);
            }

            return EXIT_SUCCESS;
        }

    }

return EXIT_SUCCESS;
}


int setSvgFilename(char* filename,char* nomFichierSvg)
{
	char *chaine;

	
	chaine = strrchr(filename, '/');

    if ( chaine == NULL ){		// pas de caractère '/', on le rajoute
		sprintf(nomFichierSvg, "../tmp/%s.%d", filename, getpid()); // initialisation du nom de fichier  // Ajout '/'
	} else {
		sprintf(nomFichierSvg, "../tmp%s.%d", chaine, getpid()); // initialisation du nom de fichier
	}
	return(EXIT_SUCCESS);
}


list getFilenameList(int sockcli)
{
	char buf[BUF_SIZE];
	ssize_t nread;                  /* Nb bytes read on socket. */
	 list ma_liste=NULL;
	 
	while(true)
    {
		// Rempli le buffeur de '\n' pour strtok
        memset( buf, 0, BUF_SIZE);
		
        // version avec recv
		// if ( (nread = recv(sockcli, buf, BUF_SIZE, 0)) == -1 )
        if ( (nread = read(sockcli, buf, BUF_SIZE)) == -1 )	// Lit le socket
        {
            perror(" main()/read()\n");
            fflush(stderr);
            printf("[%d] Echec de lecture sur la socket.\n", getpid() );
            fflush(stdout);
            exit(EXIT_FAILURE);
        }
       
        fflush(stdout);
        
        
        if ( nread < BUF_SIZE )
        {


            fflush(stdout);
            ma_liste = str2List(buf);

            return ma_liste;
        }
    }
}


list str2List(char * str)
{
	list ma_liste = NULL;
	char * ptn;
	  ptn = strtok (str,"\n");
	  while (ptn != NULL)
	  {
		ma_liste = add_tail(ptn,ma_liste);
		ptn = strtok (NULL, "\n");
	  }
  return ma_liste;
}	




