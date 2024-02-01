#ifndef SESSION_H
#define SESSION_H

#define MAX_BUFF 1024

typedef struct {
    int socket;    
    int mode;     
    struct sockaddr_in addrSrv;  
} sock_t;

<<<<<<< HEAD
=======
typedef struct {
	int nb; 
	char msg[MAX_BUFF]; 
} req_t;
>>>>>>> 53e2ec8222278bfd624d2b2b2fca830e301b9dbd
/********************************************************************************************************************************************************************************************************
	fonction	:	sock_t creerSocket(int mode)
	brief		: 	Fonction de créationo dune socket dans un mode donné	
	param		: 	mode = fournit le mode de la socket à créer (SOCK_DGRAM, SOCK_STREAM 
	result		: 	retourne la socket crée
********************************************************************************************************************************************************************************************************/
sock_t creerSocket(int mode); 
/********************************************************************************************************************************************************************************************************
	fonction	:	struct sockaddr_in createAdr(char *addrIP, short port) 
	brief		: 	Préparation de l’adressage du service (d’appel)
	param		: 	addrIP = adresse IP de l'interface utilisée 
				port = port
	result		: 	Retourne l'adresse crée
********************************************************************************************************************************************************************************************************/
struct sockaddr_in createAdr(char * addrIP, short port); 
/********************************************************************************************************************************************************************************************************
	fonction	:	int creerSocketAdresse(char * addrIP, int mode, short port)
	brief		: 	Création dune socket dans un mode donné, préparation de l’adressage du service (d’appel) et	association de l’adressage préparé avec la socket d’écoute
	param		: 	addrIP = adresse IP de l'interface utilisée 
				mode = fournit le mode de la socket à créer (SOCK_DGRAM, SOCK_STREAM)
				port = port
	result		: 	numéro de pipe descriptor de la socket créée
********************************************************************************************************************************************************************************************************/
sock_t creerSocketAdresse(char * addrIP, int mode, short port);
/********************************************************************************************************************************************************************************************************
	fonction	:	void fermerSocket(int sock);  
	brief		: 	Fermeture d'une socket 
	param		: 	sock = socket à fermer 
	result		: 	
********************************************************************************************************************************************************************************************************/
void fermerSocket(sock_t sock); 

//----------------------------------------------------------------------------- Cas stream ----------------------------------------------------------------------------------------------------------//
/********************************************************************************************************************************************************************************************************
	fonction	:	int creerSocketEcoute(char * addrIP, short maxClts, short port)
	brief		: 	Mise en écoute de la socket
	param		: 	addrIP = adresse IP de l'interface utilisée 
				maxClts = nombre maximum de client que le serveur peut accepter
				port = port
	result		: 	numéro de pipe descriptor de la socket créée
********************************************************************************************************************************************************************************************************/
sock_t creerSocketEcoute(char * addrIP, short maxClts, short port); 
/********************************************************************************************************************************************************************************************************
	fonction	:	int creerSocketConnect(char * addrIP, short port); 
	brief		: 	Création d'une socket qui réalise une demande de connection
	param		: 	addrIP = adresse IP de l'interface utilisée 
				port = port
	result		: 	numéro de pipe descriptor de la socket créée
********************************************************************************************************************************************************************************************************/
sock_t creerSocketConnect(char * addrIP, short port); 
/********************************************************************************************************************************************************************************************************
	fonction	:	int creerSocketConnect(char * addrIP, short port); 
	brief		: 	Création d'une socket d'échange
	param		: 	socketEchange = numéro de pipe de la socket d'écoute 
				addrIP = adresse IP de l'interface utilisée 
				port = port
				*clt = adresse client 
	result		: 	numéro de pipe descriptor de la socket créée
********************************************************************************************************************************************************************************************************/
sock_t creerSocketEchange(sock_t sockEcoute, char * addrIP, short port, struct sockaddr_in *clt); 
/********************************************************************************************************************************************************************************************************
	fonction	:	void envoyerMessSTREAM(int sock, char * mess);  
	brief		: 	Envoi d'un message en mode STREAM
	param		: 	sock = socket d'échange à utiliser pour l'envoi
				mess = Contenu du message à envoyer  
	result		: 	
********************************************************************************************************************************************************************************************************/
void envoyerMessSTREAM(sock_t sock, char * mess); 
/********************************************************************************************************************************************************************************************************
	fonction	:	void recevoirMessSTREAM(sock_t sock, char * mess);  
	brief		: 	Reception d'un message en mode STREAM
	param		: 	sock = socket d'échange à utiliser pour la reception
				mess = Chaine contenant le message receptionné  
				taille = Taille de la variable de reception du message
	result		: 	
********************************************************************************************************************************************************************************************************/
void recevoirMessSTREAM(sock_t sock, char *msg); 

//------------------------------------------------------------------------------ Cas DGRAM ---------------------------------------------------------------------------------------------------------//
/********************************************************************************************************************************************************************************************************
	fonction	:	void recevoirMessDGRAM(sock_t sock, char *msg)
	brief		: 	Reception d'un message en mode INET
	param		: 	sock = socket d'échange à utiliser pour l'envoi
				msg = Contenu du message receptionné
	result		: 	
********************************************************************************************************************************************************************************************************/
void recevoirMessDGRAM(sock_t sock, char *msg);  
/********************************************************************************************************************************************************************************************************
	fonction	:	void envoyerMessDGRAM(sock_t sock, char *msg)
	brief		: 	Envoi d'un message en mode INET
	param		: 	sock = socket d'échange à utiliser pour l'envoi
				buff = Contenu du message envoyé 
	result		: 	
********************************************************************************************************************************************************************************************************/
void envoyerMessDGRAM(sock_t sock, char *msg); 

#endif /* SESSION_H */