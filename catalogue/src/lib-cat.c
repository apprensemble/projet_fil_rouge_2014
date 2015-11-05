/************************************************************************
*
Projet: Projet du master IDL – 2014
*
*************************************************************************
*
* Camille Di Pietro - Thierry Alaux - Michel Sérieys
*
*************************************************************************/

#include "lib-cat.h"
 int cpt_elem = 0;

// Retourne 1 si la liste est vide sinon 0
int is_empty(list l)
{
    return l == NULL;
}


list create_element(char* e, list l)
{
    list r = (list) malloc(sizeof(struct list_t));
    r->value = e;
    r->tail = l;
    return r;
}

// Libère la mémoire occupée par la liste
void free_list(list l)
{
    if(!is_empty(l))
    {
    	free(l->value);
        free_list(l->tail);
        free(l);
    }
}

// Calcule la longueur de la liste
int length(list l)
{
    if(is_empty(l)) return 0;
    else return 1 + length(l->tail);
}


// renvoie le nième élément d'une liste
char* get_value_at_pos(int i, list l)
{
    assert(!is_empty(l));
    assert(i>=0);
    if(i == 0) return l->value;
    else return get_value_at_pos(i - 1, l->tail);
}


//insère un élément à la position indiquée
list insert(char* e, int i, list l)
{
    assert(i>=0);
    if(i == 0) return create_element(e,l);
    assert(!is_empty(l));
    l->tail = insert(e, i - 1, l->tail);
    return l;
}

// Affiche le contenu de la liste
void print_list(list l)
{
	list elem;
	int cpt_elem;
	
	elem=l;
	cpt_elem=0;
	
    while(!is_empty(elem))
    {
        printf("%4.1i : %s \n", cpt_elem, elem->value);
        cpt_elem++;
        elem = elem->tail;
    }
}

static void print_elem_list(char * value)
{
	printf("%s \n", value);
}


list add_tail(char* valeur, list liste)
{
    /* On crée un nouvel élément */
    list nouvelElement = (list) malloc(sizeof(struct list_t));
    if(nouvelElement == NULL)
    {
        fprintf(stderr,"Allocation impossible");
        exit(EXIT_FAILURE);
    }


    /* On assigne la valeur au nouvel élément */
    /* Allouer une chaine pour la copie */
    nouvelElement->value = (char *) malloc(sizeof(char) * strlen(valeur));
    strcpy(nouvelElement->value,valeur);


    /* On l'ajoute en fin, donc aucun élément ne va suivre */
    nouvelElement->tail = NULL;

    if(liste == NULL)
    {
        /* Si la liste est vide on renvoie l'élément créé */
        return nouvelElement;
    }
    else
    {
        /* Sinon, on parcourt la liste à l'aide d'un pointeur temporaire et on
        indique que le dernier élément de la liste est relié au nouvel élément */
        list temp=liste;
        while(temp->tail != NULL)
        {
            temp = temp->tail;
        }
        temp->tail = nouvelElement;
        return liste;
    }
}


int ls_rep(list ma_liste, char* chemin)
{
    DIR *dp = opendir(chemin);
    if (dp)
    {
        struct dirent *entry;
        while ( (entry = readdir(dp) ))
        {
            if (!(strcmp(".", entry->d_name) && strcmp("..", entry->d_name)))
                continue;
            ma_liste = add_tail(entry->d_name,ma_liste);

        }
        closedir(dp);

        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

char* menu_choice (list ma_liste) {
int choix;
    printf("\"0\" pour quitter ou le numéro du fichier sélectionné ( 0 à %i ) : ", length(ma_liste)-1);
    while (scanf("%d", &choix) != 1 || choix<0 || choix>length(ma_liste)-1)
    {
        while (getchar() != '\n');
        printf ("Nouvel essai: ");
    }

    if (choix == 0)
    {
	free_list(ma_liste);
    exit(0);
    }

    printf ("Vous avez sélectionné : %s\n", get_value_at_pos(choix, ma_liste));
    return get_value_at_pos(choix, ma_liste);

}

