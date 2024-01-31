#include "proto.h"

void dialogueClt (sock_t sd, struct sockaddr_in clt); 

int main()
{		
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
		CHECK(pidFils = fork(), "fork"); 
		
		if (pidFils == 0){
			// FERMER LA SOCKET D'ECOUTE 
			fermerSocket(se); 
			
			// Dialogue avec le client
			dialogueClt (sd , clt);
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

void dialogueClt (sock_t sd, struct sockaddr_in clt) 
{
	int choix; 
	req_t requete; 
	
	 do {
		recevoir (sd, &requete, deSerial); 		
		printf("Message du client :: "); 
		printf("%d : %s\n", requete.nb, requete.msg); 
	} while ( requete.nb != 0);
}
