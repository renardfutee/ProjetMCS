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
        	scanf("%s", username);
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
	int choix, result; 
	char reponse;    
	char adv[MAX_BUFF];  
	reqSimple_t requete; 

	// L'utilisateur commence par se connecter ! 
	sendRequete(sd, CONNECT, username, NULL, NULL);  
	receiveReponse(sd);
	
	do{
		printf("<--------------- MENU --------------->\n"); 
		printf("0 - Quitter le jeu\n"); 
		printf("1 - Commencer une nouvelle partie\n"); 
		scanf("%d", &choix); 
		
		switch(choix){
			case 0: 
				printf("Vous avez choisit de vous déconnecter\n"); 
				sendRequete(sd, DISCONNECT, username, NULL, NULL);
				receiveReponse(sd);
			break; 
			case 1:
				printf("Vous avez choisit de lancer une nouvelle partie\n\n Voici les joueurs disponibles : \n -----------------------------------------\n"); 
				sendRequete(sd, GET_USRS, username, NULL, NULL);
				receiveReponse(sd);
				printf("-----------------------------------------\n"); 
				
				do {
					printf("Veuillez entrer le nom du joueur que vous voulez défier: "); 
					scanf("%s", adv);
					// printf("Nom du joueur choisit : %s\n", adv);
					sendRequete(sd, CHECK_PSEUDO, adv, NULL, NULL); 
					result = receiveReponse(sd).nb; 
				}while(result == 0); 
				
				sendRequete(sd, NOUV_PART, username, adv, NULL);
				printf("Id partie: "); 
				result = atoi(receiveReponse(sd).msg); 
				printf("\n\n-------------------------------------------------\n|\tPARTIE %s contre %s\t\t|\n-------------------------------------------------\n", username, adv); 
				printf("|\t\t\t\t\t\t|\n|\t\tManche 1\t\t\t|\n|\tVoulez-vous jouer cette manche ?\t|\n|\t\t\t\t\t\t|\n-------------------------------------------------\n[Y|N]--->"); 
				scanf(" %c", &reponse);

				if (reponse == 'y' || reponse == 'Y') {
        				sendRequete(sd, LANCER_MANCHE, username, result, 1);

        				do {
        					result = receiveReponse(sd).nb;
        				}while(result != 0); 

        			}
    				
			break; 
			default:  
			break ; 
		}
	} while(choix != 0); 
}
