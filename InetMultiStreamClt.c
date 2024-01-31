#include "StreamInc.h"

int main(int argc, char *argv[]) {
    int se, sd;
    char *addrIP; 
    int port; 
    struct sockaddr_in svc, clt;
    socklen_t cltLen;

    if(argc < 3) {
        printf("Informations manquantes. Rappel du format:: ./cltMulti adrIP port"); 
        return 0; 
    }
    addrIP = argv[1]; 
    port = atoi(argv[2]); 
    printf("Adresse IP ==> %s \t Port ==> %d\n",  argv[1], port); 

    // Création de la socket d’appel et de dialogue
    CHECK(se=socket(PF_INET, SOCK_STREAM, 0), "Can't create");

    // Préparation de l’adressage du service à contacter
    svc.sin_family = PF_INET;
    svc.sin_port = htons (port);
    svc.sin_addr.s_addr = inet_addr(addrIP);
    memset(&svc.sin_zero, 0, 8);

    // Demande d’une connexion au service
    CHECK(connect(se, (struct sockaddr *)&svc, sizeof svc) , "Can't connect");

    // Dialogue avec le serveur
    dialogueSrv (se, &svc);
    close(se);

    return 0;
}

void dialogueSrv (int sd, struct sockaddr_in *srv) {
	char reponse[MAX_BUFF];
	
	// Envoi du message MSG au serveur : la réponse sera OK
	CHECK(write(sd, MSG, strlen(MSG)+1), "Can't send");
	CHECK(read(sd, reponse, sizeof(reponse)), "Can't send");
	
	// Envoi du message ERR au serveur : la réponse sera NOK
	CHECK(write(sd, ERR, strlen(ERR)+1), "Can't send");
	CHECK(read(sd, reponse, sizeof(reponse)), "Can't send");
	
	// Envoi du message BYE au serveur : la réponse sera la fin du dialogue
	CHECK(write(sd, BYE, strlen(BYE)+1), "Can't send");
	CHECK(read(sd, reponse, sizeof(reponse)), "Can't send");
}
