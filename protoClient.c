#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#include "data.h"

void dialogueSrv (sock_t sd, struct sockaddr_in *srv, char *username);

int main(int argc, char *argv[]){

    struct sockaddr_in svc, clt;
	socklen_t cltLen;
	sock_t se; 
    char username[MAX_BUFF];
    printf("Client\n"); 
		
	if(argc < 2) {
        printf("Veuillez vous identifier \n"); 
        fgets(username, MAX_BUFF, stdin);
	}else{
        sscanf(argv[1], "%s", username);
    }
    
    printf("Bienvenue %s\n", username);
		
	// Création de la socket de connection au systme 
	se = creerSocketConnect("127.0.0.1", 5000); 
		
	// Dialogue avec le serveur
	dialogueSrv (se, &svc, username);
	fermerSocket(se);
}

void dialogueSrv (sock_t sd, struct sockaddr_in *srv, char *username) {
	int choix; 
	char reponse[MAX_BUFF]; 
	req_t requete; 

    requete.nb = 001;
    strcpy(requete.msg, username); 

    envoyer(sd, &requete, serial); 

    requete.nb = -1;
    strcpy(requete.msg, "nfenfuebfube"); 
    envoyer(sd, &requete, serial); 

    recevoir(sd, &requete, deSerial);
    printf("Reponse : %s\n", requete.msg);

    switch(requete.nb) {
        case 1: 
            printf("Création nouvel utilisateur\n"); 

        break; 
        case 0: 
            printf("Connexion à votre compte\n"); 
            
            
        break; 
        default: 
            printf("Connexion impossible\n");
            exit(-1);
        break; 
    }
	
    choix = displayMenu();

    switch(choix) {
        case 1: 
            printf("Création d'une nouvelle partie\n");
            affichageJoueur();
        break; 
        case 2: 
            printf("Option 2\n"); 
        break; 
        case 3: 
            printf("Option 3\n"); 
        break; 
        case 4: 
            printf("Exit\n"); 
            exit(-1);
        break; 
        default: 
            printf("Choix impossible\n"); 
        break; 
    }


	// do {
	// 	printf("Choisir le message :\n");
	// 	printf("1 : %s\n", MSG); 
	// 	printf("2 : %s\n", ERR); 
	// 	printf("3 : %s\n", BYE);  
	// 	scanf("%d", &choix);
	// 	printf("Vous avez choisi %d\n", choix);
	
	// 	switch(choix) {
	// 		case 1: 
	// 			requete.nb = 100; 
	// 			strcpy(requete.msg, "Je dis que \"le fond de l’eau est clair par ici ! Où ça ?\"");
	// 		break;
	// 		case 2: 
	// 			requete.nb = 200; 
	// 			strcpy(requete.msg, "Requête ou réponse non reconnue !");
	// 		break; 
	// 		case 3:
	// 			requete.nb = 000; 
	// 			strcpy(requete.msg, "Au revoir et à bientôt ...");
	// 		break; 
	// 		default: 
	// 			printf("Choix impossible, fin\n"); 
	// 		break; 
	// 	}
		
	// 	envoyer(sd, &requete, serial); 
	// } while(choix != 3); 
}

int displayMenu() {
    printf("Menu:\n");
    printf("1. Créer une nouvelle partie\n");
    printf("2. Option 2\n");
    printf("3. Option 3\n");
    printf("4. Exit\n");

    printf(" \n Choix : ");
    scanf("%d", &choix);

    return choix;
}
