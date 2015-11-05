LIBNAME=lib-cat
DIR_INCLUDE=.
DIR_LIB=../lib


# A adapter suivant le système d'exploitation
# Fedora : /usr/lib64	-	pour Ubuntu : /usr/lib
SYS_DIR_LIB=/usr/local/lib
#SYS_DIR_LIB=/usr/lib64


SYS_DIR_INCLUDE=/usr/include

.PHONY: all build clean reset install

#all: build install

build: $(LIBNAME).c $(LIBNAME).h
	gcc --shared -o $(DIR_LIB)/$(LIBNAME).so $(LIBNAME).c -fPIC

clean:
	\rm -f *~

# Supprime la lib et le fichier .h
uninstall: clean
	\rm -f $(SYS_DIR_LIB)/$(LIBNAME).so
	\rm -f $(SYS_DIR_INCLUDE)/$(LIBNAME).h

install:
	# install de la lib so sur le systeme : $(SYS_DIR_LIB)/$(LIBNAME).so
	# A faire pour accéder à la lib : export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
	install -m 755 $(DIR_LIB)/$(LIBNAME).so $(SYS_DIR_LIB)/
	#---------------------------------------------------------------------------
	# install des headers de la lib sur le systeme : $(SYS_DIR_INCLUDE)/$(LIBNAME).h
	install -m 644 $(DIR_INCLUDE)/$(LIBNAME).h $(SYS_DIR_INCLUDE)/
