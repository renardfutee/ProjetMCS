#include "StreamInc.h"

void procFils(int se, int sd, struct sockaddr_in clt); 
void dialogueClt (int sd, struct sockaddr_in clt) ; 
void traiterCHLD(int sig);

int main() {
	int se, sd;
	struct sockaddr_in svc, clt;
	struct sigaction action;	// Signal
	socklen_t cltLen;
	pid_t pidFils; 		//PID du processus fils créé 
	
	action.sa_handler =traiterCHLD;
	action.sa_flags = 0;
	CHECK(sigemptyset(&action.sa_mask),"__sigemptyset__");
	CHECK(sigaction(SIGINT, &action, NULL), "__sigaction__");
	
	// CREATION DE LA SOCKET D'ECOUTE
	// Création de la socket de réception d’écoute des appels
	CHECK(se=socket(PF_INET, SOCK_STREAM, 0), "Can't create");
	// Préparation de l’adressage du service (d’appel)
	svc.sin_family = PF_INET;
	svc.sin_port = htons (5000);
	svc.sin_addr.s_addr = inet_addr("127.0.0.1");
	//svc.sin_addr.s_addr = inet_addr("192.168.119.129");
	memset(&svc.sin_zero, 0, 8);
	// Association de l’adressage préparé avec la socket d’écoute
	CHECK(bind(se, (struct sockaddr *) &svc, sizeof svc) , "Can't bind" );
	
	// METTRE EN ECOUTE LA SOCKET
	CHECK(listen(se, 5) , "Can't calibrate" );
	PAUSE("socket crée");
	
	// ATTENDRE UNE CONNEXION 
	// Boucle permanente de service
	while (1) {
		// Attente d’un appel
		cltLen = sizeof(clt);
		CHECK(sd=accept(se, (struct sockaddr *)&clt, &cltLen) , "Can't connect sd");
		
		CHECK(pidFils = fork(), "fork"); 
		if (pidFils == 0){
			// procFils(se, sd, clt); 
			PAUSE("1 client reçu"); 
			// FERMER LA SOCKET D'ECOUTE 
			close(se);
			
			// Dialogue avec le client
			dialogueClt (sd , clt);
			// FEMRER LA SOCKET D'ECHANGE
			close(sd);
			exit(0); 
		}	
		// FEMRER LA SOCKET D'ECHANGE
		close(sd);
		
	}
	// FERMER LA SOCKET D'ECOUTE 
	close(se);

	return 0;
}

void procFils(int se, int sd, struct sockaddr_in clt)
{
	PAUSE("1 message reçu"); 
	// FERMER LA SOCKET D'ECOUTE 
	close(se);
	// Dialogue avec le client
	dialogueClt (sd , clt);
	// FEMRER LA SOCKET D'ECHANGE
	close(sd);	
	sleep(20); 
	exit(NULL); 
}

/*
void dialogueClt (int sd, struct sockaddr_in clt) {
	char requete[MAX_BUFF];
	do {
		read(sd, buffer, sizeof(buffer));
		sscanf (buffer, "%s:%s",requete, buffer);
		switch (atoi(requete)) {
			case 0 : printf("Au revoir\n"); 
			break;
			
			case 100 : write(sd, OK, strlen(OK)+1);
			printf("OK : message recu %s\n", buffer);
			break;
			
			default : write(sd, NOK, strlen(NOK)+1);
			printf("NOK : message recu %s\n", buffer); 
			break;
		}
	} while ( atoi(requete) != 0);
} */

void dialogueClt(int sd, struct sockaddr_in clt) {
    char username[MAX_BUFF];
    
    // Lire le nom d'utilisateur envoyé par le client
    read(sd, username, sizeof(username));
    printf("Nom d'utilisateur reçu : %s\n", username);

    // Envoyer un message de bienvenue avec le nom d'utilisateur
    char welcomeMsg[MAX_BUFF];
    snprintf(welcomeMsg, sizeof(welcomeMsg), "Bienvenue, joueur %s !", username);
    write(sd, welcomeMsg, strlen(welcomeMsg) + 1);
}

void traiterCHLD (int sig) {
	int pidFils = wait(NULL);
}
