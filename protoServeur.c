#include "proto.h"

int main()
{		
	int nbClient = 0; 
	char buffer[MAX_BUFF];
	struct sockaddr_in svc, clt, addrSrv;
	socklen_t cltLen, lenAddr = sizeof(struct sockaddr_un);
	pid_t pidFils; 		//PID du processus fils créé 
	sock_t se, sd, sock; 
		
	// CREATION DE LA SOCKET D'ECOUTE
	se = creerSocketEcoute("127.0.0.1", 5, 5000); 
	printf("SERVEUR APPLICATION LANCE !! \n--------------------------\n"); 
		
	// ATTENTE D'UNE CONNEXION 
	// Boucle permanente de service
	while (1) {
		// Attente d’un appel
		sd = creerSocketEchange(se, "127.0.0.1", 5000, &clt); 
		nbClient ++; 
		
		CHECK(pidFils = fork(), "fork"); 
		if (pidFils == 0){
			// procFils(se, sd, clt); 
			PAUSE("1 client reçu"); 
			// FERMER LA SOCKET D'ECOUTE 
			fermerSocket(se); 
			
			// Dialogue avec le client
			//dialogueClt (nbClient, sd , clt);
			printf("UN CLIENT RECU\n\n"); 
			// FEMRER LA SOCKET D'ECHANGE
			fermerSocket(sd); 
			exit(0); 
		}	
		// FEMRER LA SOCKET D'ECHANGE
		fermerSocket(sd); 
		
	}
	// FERMER LA SOCKET D'ECOUTE 
	fermerSocket(se); 
}
