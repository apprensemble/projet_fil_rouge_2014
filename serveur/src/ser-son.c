/************************************************************************
*
* Projet: Projet du master IDL – 2014
*
*************************************************************************
*
* Camille Di Pietro - Thierry Alaux - Michel Sérieys
*
*************************************************************************/

#include "ser-son.h"  /* Création du processus fils traitant la demande du client */


int sonProcess(int sockcli, struct sockaddr * addrcli, socklen_t size_addrcli, list ma_liste)
{
    char buf[BUF_SIZE];             /* Buffer for storing bytes read on socket. */
    char *filename;					/* Nom du fichier à afficher */
    ssize_t nread;                  /* Nb bytes read on socket. */
    FILE * fd;
    int rfils;                      /* Temporary return code */

    printf("[%d] .............. Arguments ok. Tentative de connexion au client. \n",getpid());
    fflush(stdout);

    /* 5. Looking at information on the client **********************************/

    char host[NI_MAXHOST], service[NI_MAXSERV];

    if((rfils=getnameinfo( addrcli, size_addrcli, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV)) == 0 )
    {
        printf("[%d] .............. Client %s:%s Identification ok.\n",getpid() , host, service);
        fflush(stdout);
    }
    else
    {
        printf("[%d] .............. Echec lors de l'appel à getnameinfo: %s.\n",getpid() , gai_strerror(rfils));
        fflush(stdout);
    }


    /* 6. Send list of awailable file **********************************/
    sendFilenameList(sockcli, ma_liste);



    /* 7. Get the filename from the client. ************************************/
    while(true)
    {

        if ( (nread = read(sockcli, buf, sizeof(buf)+1)) < 0 )
        {
            fflush(stdout);
            perror("..............  main()1/read()\n");
            fflush(stderr);
            printf("[%d] .............. Erreur de lecture du socket.\n",getpid());
            fflush(stdout);
            exit(EXIT_FAILURE);
        }

        // Choix "0 = Quitter" par le client, le message reçu est vide
        if ( nread == 0 )
        {
            break;
        }


        filename=buf;


        /* 8. Send the file content to the client ******************************************/
        char full_name[100];
        strcpy(full_name,TXT_CATALOG_PATH);
        strcat(full_name, filename);

        printf("\n[%d] .............. Fichier demandé : \"%s\".\n",getpid() , full_name);
        fd = openFile(sockcli, full_name);
        if (fd == NULL)
        {
            return EXIT_FAILURE;
        }
        else
        {
            sendFileContent( sockcli, fd, filename);
        }
    }
    return EXIT_SUCCESS;
}




FILE* openFile(int sockcli, char *filename)
{
    FILE * fd;

    /* Open the file */
    if ((fd = fopen(filename,"r")) == NULL)
    {
        fprintf(stderr,"[%d] .............. Impossible d'ouvrir le fichier \"%s\".\n",getpid(), filename);
        send( sockcli, "ERREUR. Impossible d'ouvrir le fichier. \n", 42, 0);
        return NULL;
    }

    printf("[%d] .............. Ouverture du fichier \"%s\".\n",getpid() , filename);
    return fd;
}


/* Envoi de la liste des fichiers disponibles */
void sendFilenameList(int sockcli,list ma_liste)
{
    list elem;
    char filenameList[1024];

    elem=ma_liste;

    fflush(stdout);
    while(!is_empty(elem))
    {
        sprintf(filenameList,"%s%s\n",filenameList,elem->value);
        fflush(stdout);
        elem = elem->tail;
    }
    send(sockcli, filenameList, strlen(filenameList), 0);
    //write(sockcli, filenameList, strlen(filenameList));

}





void sendFileContent(int sockcli, FILE * fd, char *filename)
{

    char buf[BUF_SIZE];
    int nb_lignes = 0;

    // Affiche et compte les lignes du fichier
    //while(fgets(buf, BUF_SIZE, fd) != NULL)
    while( fgets (buf, BUF_SIZE, fd) != NULL)
    {
        send(sockcli, buf, strlen(buf), 0);
        //write(sockcli, buf, strlen(buf));
        nb_lignes++;
    }
    printf("[%d] .............. Transfert de \"%s\" terminé (%d ligne(s)).\n",getpid() , filename, nb_lignes);
    fflush(stdout);

    // fermeture du fichier
    if(fclose(fd) == EOF)
    {
        printf("[%d] .............. Problème de fermeture du fichier \"%s\".\n",getpid() , filename);
        exit(1);
    }
}
