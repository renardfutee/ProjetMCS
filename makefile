# Makefile

all: clt svc trouverFichier coreApp

clt: protoClient.c reqRep.c session.c data.c coreApp.c
	gcc -o clt protoClient.c session.c data.c reqRep.c coreApp.c -DCLIENT -ljansson

svc: protoServeur.c  reqRep.c session.c data.c coreApp.c
	gcc -o svc protoServeur.c session.c data.c reqRep.c coreApp.c -DSERVEUR -ljansson

trouverFichier: trouverfichier.c
	gcc -o trouverFichier trouverfichier.c -ljansson licuuc -licui18n

coreApp: coreApp.c
	gcc -o coreApp coreApp.c -ljansson

clear:
	rm -f *.o clt svc coreApp trouverFichier
