#ifndef CAT_MENU_H_INCLUDED
#define CAT_MENU_H_INCLUDED

#include <stdlib.h> /* EXIT_FAILURE...*/
#include <stdio.h>  /* printf, fopen... */
#include <stdbool.h>
#include <assert.h>
#include <dirent.h>
#include <string.h>

struct list_t
{
    char* value;
    struct list_t* tail;
};

typedef struct list_t* list;

typedef void (* traitement_list_ptn_func)(char * value) ;

/* execute une fonction sur chaque element de la liste */
void foreach_list(list l, traitement_list_ptn_func func);

void print_list_fe(list l);

list create_element(char* e, list l);
int is_empty(list l);
void free_list(list l);
int length(list l);
char* nth(int i, list l);
list insert(char* e, int i, list l);
void print_list(list l);
list add_tail(char* v, list l);
int ls_rep(list l, char * path);
char * menu_choice (list l);


#endif // CAT_MENU_H_INCLUDED
