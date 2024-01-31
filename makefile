clt: useLibNet.c session.c data.c
	gcc -o clt useLibNet.c session.c data.c -DCLIENT

srv: useLibNet.c session.c data.c
	gcc -o svc useLibNet.c session.c data.c -DSERVEUR

trouverFichier: trouverfichier.c
	gcc -o trouverFichier trouverfichier.c -ljansson

clear:
	rm -f *.o clt svc
