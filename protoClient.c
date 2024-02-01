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
	sendRequete(sd, "connect\0", username);  
	
    	/*requete.nb = 001;
    	strcpy(requete.msg, username); 
    	envoyer(sd, &requete, serial); */

    	requete.nb = -1;
    	strcpy(requete.msg, "nfenfuebfube"); 
    	envoyer(sd, &requete, serial); 
    	recevoir(sd, &requete, deSerial);
    	printf("Reponse : %s\n", requete.msg);
}
