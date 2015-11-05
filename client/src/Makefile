#		Makefile pour la partie CLIENT


#---------------------------------------------------------
# EXEDIR = Répertoire d'accueil de l'exécutable
#---------------------------------------------------------
EXEDIR=../bin


#---------------------------------------------------------
# HEADDIR = Répertoire d'accueil des fichiers d'entêtes
#---------------------------------------------------------
HEADDIR=../include


#---------------------------------------------------------
# OBJDIR = Répertoire d'accueil des fichiers compilés
#---------------------------------------------------------
OBJDIR=../obj


#---------------------------------------------------------
# Nom de l'exécutable
# Préfixe des fichiers de l'application
# Nom du fichier de dépendences
#---------------------------------------------------------
EXE=tcp-client
CODE=cli
MAKEDEPFILE=Dependencies-$(CODE)


#---------------------------------------------------------
# Nom du compilateur
#---------------------------------------------------------
CC=gcc


#---------------------------------------------------------
# Nom du générateur du fichier de dépendances
#---------------------------------------------------------
MAKEDEP=gccmakedep -f $(MAKEDEPFILE)


#---------------------------------------------------------
# CFLAGS regroupe les options pour le compilateur
# -g : déboguage
# -I : include
# -O : optimisation
# -w -Wall : suppression des warnings
#---------------------------------------------------------
CFLAGS += -I$(HEADDIR) -L /usr/lib64/ -l -cat -Wall


#---------------------------------------------------------
# LDFLAGS regroupe les options de l'édition de liens
#---------------------------------------------------------
LDFLAGS += -L /usr/lib64/ -l -cat



#---------------------------------------------------------
# DEFINITION D'UNE REGLE GENERIQUE
#
# Construction de la variable SOURCE en cherchant tous les fichiers
# répondant au patern ser*.c dans le répertoire 
# SOURCE=$(wildard $(CODE)*.c) 
#---------------------------------------------------------
SOURCES=$(wildcard $(CODE)-*.c)


#---------------------------------------------------------
# DEFINITION D'UNE REGLE GENERIQUE
#
# Construction du nom des fichiers OBJETS à partir de tous les '.c'.
# Pour chaque fichier '.c' contenu dans 'SOURCES' on déclare un
# fichier objet de même nom portant l'extension '.o'.
#---------------------------------------------------------
OBJETS=$(SOURCES:%.c=$(OBJDIR)/%.o)


#---------------------------------------------------------
# les dépendances déclarées après 'all' seront systématiquement
# reconstruites sans vérifier s'il y a eu des modifications.
#---------------------------------------------------------
.PHONY: all clean reset list help depend


#---------------------------------------------------------
# Génération de l'exécutable
# $^ : La liste des dépendances
# $@ : Le nom de la cible
#---------------------------------------------------------
$(EXEDIR)/$(EXE): $(OBJETS)
	test -d $(EXEDIR) || mkdir $(EXEDIR)
	$(CC) $(LDFLAGS) -o $@ $^


#---------------------------------------------------------
# Génération des différents fichiers objet '*.o' a partir des '*.c'.
# Si le répertoire n'existe pas, on le crée
#---------------------------------------------------------
$(OBJDIR)/%.o : %.c
	test -d $(OBJDIR) || mkdir $(OBJDIR)
	$(CC) $*.c -c $(CFLAGS) -o $@


#---------------------------------------------------------
# Regroupe le lancement de l'ensemble des exécutables à produire.
# (fichier Dépendences, fichiers objets et fichier exécutable)
#---------------------------------------------------------
all: depend $(OBJETS) $(EXEDIR)/$(EXE)


#---------------------------------------------------------
# Génération du fichier de dépendances
#---------------------------------------------------------
depend:
	$(MAKEDEP) -- $(CFLAGS) -- $(SOURCES)


#---------------------------------------------------------
# Suppression des fichiers de backup créés par l'éditeur '.c~'
#---------------------------------------------------------
clean:
	\rm -f *~


#---------------------------------------------------------
# Suppression des fichiers objets '.o' et '.c~'
#---------------------------------------------------------
cleanobj:	clean
	\rm -f $(OBJDIR)/*.o


#---------------------------------------------------------
# Suppression du fichier exécutable
#---------------------------------------------------------
cleanexe: reset
	\rm -f $(EXEDIR)/$(EXE)


#---------------------------------------------------------
# Suppression de tout sauf les fichiers '.c', '.h' et Makefile
#---------------------------------------------------------
cleanall:	clean
	\rm -fr $(OBJDIR)
	\rm -fr $(EXEDIR)
	\rm -f $(MAKEDEPFILE)
	\rm -fr ../tmp


#---------------------------------------------------------
# Affiche le contenu des répertoires: 'src' + 'include' + 'obj' + 'bin'
# appartenant au projet.
#---------------------------------------------------------
list:
	@echo SOURCES	= $(wildcard $(CODE)*.c)
	@echo HEADERS	= $(wildcard $(HEADDIR)/$(CODE)*.h)
	@echo DEPEND	= $(wildcard Dependencies-$(CODE))
	@echo OBJETS	= $(wildcard $(OBJDIR)/$(CODE)*.o)
	@echo EXEC	= $(wildcard $(EXEDIR)/$(EXE))


#---------------------------------------------------------
# Affiche la liste des options disponibles
#---------------------------------------------------------
help:
	@echo
	@echo
	@echo "			Options disponibles"
	@echo
	@echo "-------------------------------------------------------------------"
	@echo "    depend	: Création du fichier de dépendences: Dependencies-$(CODE)"
	@echo "    list	: Liste les fichiers du projets"
	@echo "    clean	: Effacement des fichiers de sauvegarde de l'éditeur"
	@echo "    cleanobj	: Effacement des fichiers objets"
	@echo "    cleanexe	: Effacement des fichiers objets et de l'exécutable"
	@echo "    cleanall	: Effacement de tout sauf des fichiers '.c', '.h' et Makefile"
	@echo "-------------------------------------------------------------------"
	@echo 
