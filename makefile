# Makefile

all: clt svc trouverFichier

clt: protoClient.c session.c data.c
	gcc -o clt protoClient.c session.c data.c -DCLIENT

svc: protoServeur.c session.c data.c
	gcc -o svc protoServeur.c session.c data.c -DSERVEUR

trouverFichier: trouverfichier.c
	gcc -o trouverFichier trouverfichier.c -ljansson

clear:
	rm -f *.o clt svc
