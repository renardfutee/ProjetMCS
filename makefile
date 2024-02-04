# Makefile

all: clt svc trouverFichier coreApp

clt: protoClient.c reqRep.c session.c data.c
	gcc -o clt protoClient.c reqRep.c  session.c data.c -DCLIENT

svc: protoServeur.c  reqRep.c session.c data.c
	gcc -o svc protoServeur.c reqRep.c session.c data.c -DSERVEUR

trouverFichier: trouverfichier.c
	gcc -o trouverFichier trouverfichier.c -ljansson

coreApp: coreApp.c
	gcc -o coreApp coreApp.c -ljansson

clear:
	rm -f *.o clt svc coreApp trouverFichier
