#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>

#include "StreamInc.h"
#include "reqRep.h"

void dialogueClt (sock_t sd, struct sockaddr_in clt); 
void signalHandler(int signum); 

sock_t sockTemp; 

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
	rep_t rep; 
	struct sigaction sa;
	
    	sa.sa_handler = signalHandler;
    	sigemptyset(&sa.sa_mask);
    	sa.sa_flags = SA_RESTART;
    	sigaction(SIGALRM, &sa, NULL);
	
	// Le serveur reçoit d'abord une demande de connexion 
	rep = receiveRequete(sd); 
	sendRep(sd, rep); 
	sockTemp = sd; 
	
	do{
		rep = receiveRequete(sd); 
		sendRep(sd, rep); 
	}while(rep.nb != -100); 
	
	exit(0); 
}

void signalHandler(int signum) {
	rep_t rep; 

	switch(signum){
		case SIGALRM : 
			printf("Fin de l'alarme\n"); 
			rep.nb = 0; 
			strcpy(rep.msg, "Fin du temps imparti !"); 
			sendRep(sockTemp, rep);
		break; 
		default: 
		break; 
	}
}
