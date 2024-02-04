#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#include "reqRep.h"
#include "data.h"

void dialogueSrv (sock_t sd, struct sockaddr_in *srv, char *username);

int main(int argc, char *argv[]){
	char username[MAX_BUFF];
    	struct sockaddr_in svc, clt;
	socklen_t cltLen;
	sock_t se; 
		
	if(argc < 2) {
        	printf("Veuillez vous identifier : "); 
        	fgets(username, MAX_BUFF, stdin);
	}
	else
        sscanf(argv[1], "%s", username);
    
	// Création de la socket de connection 
	se = creerSocketConnect("127.0.0.1", 5000); 
	// Dialogue avec le serveur
	dialogueSrv (se, &svc, username);
	fermerSocket(se);
}

void dialogueSrv (sock_t sd, struct sockaddr_in *srv, char *username) {
	int choix; 
	char reponse[MAX_BUFF]; 
	reqSimple_t requete; 

	// L'utilisateur commence par se connecter ! 
	sendRequete(sd, CONNECT, username);  
	receiveReponse(sd);
	
	do{
		printf("<--------------- MENU --------------->\n"); 
		printf("0 - Quitter le jeu\n"); 
		printf("1 - Commencer une nouvelle partie\n"); 
		scanf("%d", &choix); 
		
		switch(choix){
			case 0: 
				printf("Vous avez choisit de vous déconnecter\n"); 
				sendRequete(sd, DISCONNECT, username);
			break; 
			case 1:
				printf("Vous avez choisit de lancer une nouvelle partie\n"); 
				sendRequete(sd, NOUV_PART, username);
			break; 
			default:  
			break ; 
		}
		
		receiveReponse(sd);
	} while(choix != 0); 
}


