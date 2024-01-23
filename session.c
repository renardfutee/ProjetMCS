#include "session.h" 

// Session: ensemble de fonctionalités permettant: 
//	- d'établir une connexion
//	- de fermer une connextion
//	- de permettre un dialogue (qcq : selon un protocole applicatif) entre C/S c'est à dore lire/écrire des message (chaine de car)
// Objectif: faire abstraction de couche 1 à 5 

#define CHECK(sts,msg) if ((sts) == -1) {perror(msg);exit(-1);}

/********************************************************************************************************************************************************************************************************
	fonction	:	int creerSocket(int mode);
	brief		: 	Fonction de création dune socket dans un mode donné	
	param		: 	mode = fournit le mode de la socket à créer (SOCK_DGRAM, SOCK_STREAM)
	result		: 	numéro de pipe descriptor de la socket créée
********************************************************************************************************************************************************************************************************/
sock_t creerSocket(int mode)
{
	sock_t sock; 
	
	// Création de socket
	CHECK(sock.socket=socket(PF_INET, mode, 0), "__SOCKET__");
	sock.mode = mode; 
	return sock; 
}
/********************************************************************************************************************************************************************************************************
	fonction	:	struct sockaddr_in createAdr(char *addrIP, short port) 
	brief		: 	Préparation de l’adressage du service (d’appel)
	param		: 	addrIP = adresse IP de l'interface utilisée 
				port = port
	result		: 	Retourne l'adresse crée
********************************************************************************************************************************************************************************************************/
struct sockaddr_in createAdr(char *addrIP, short port) 
{
	struct sockaddr_in addr;

    	// Préparation de l’adressage du service (d’appel)
    	addr.sin_family = PF_INET;
    	addr.sin_port = htons(port);
    	addr.sin_addr.s_addr = inet_addr(addrIP); // Conversion de l'adresse IP en format binaire
    	memset(&addr.sin_zero, 0, 8);
    	return addr;
}
/********************************************************************************************************************************************************************************************************
	fonction	:	void fermerSocket(sock_t sock)
	brief		: 	Fermeture d'une socket 
	param		: 	sock = socket à fermer 
	result		: 	
********************************************************************************************************************************************************************************************************/
void fermerSocket(sock_t sock)
{
	close(sock.socket); 
}
/********************************************************************************************************************************************************************************************************
	fonction	:	sock_t creerSocketAdresse(char * addrIP, int mode, short port)
	brief		: 	Création dune socket dans un mode donné, préparation de l’adressage du service (d’appel) et	association de l’adressage préparé avec la socket d’écoute
	param		: 	addrIP = adresse IP de l'interface utilisée 
				mode = fournit le mode de la socket à créer (SOCK_DGRAM, SOCK_STREAM)
				port = port
	result		: 	socket céé
********************************************************************************************************************************************************************************************************/
sock_t creerSocketAdresse(char * addrIP, int mode, short port)
{ 
	sock_t sock; 
	struct sockaddr_in addr;
	 
	sock=creerSocket(mode); 
	addr=createAdr(addrIP, port);
	CHECK(bind(sock.socket, (struct sockaddr *) &addr, sizeof addr) , "Can't bind" ); 
	sock.addrSrv = addr; 
	return sock; 	
}
//------------------------------------------------------------------------------ Cas stream ---------------------------------------------------------------------------------------------------------//
/********************************************************************************************************************************************************************************************************
	fonction	:	sock_t creerSocketEcoute(char * addrIP, short maxClts, short port)
	brief		: 	Mise en écoute de la socket
	param		: 	addrIP = adresse IP de l'interface utilisée 
				maxClts = nombre maximum de client que le serveur peut accepter
				port = port
	result		: 	socket créée
********************************************************************************************************************************************************************************************************/
sock_t creerSocketEcoute(char * addrIP, short maxClts, short port)
{
	sock_t sock; 
	struct sockaddr_in addr; 
	
	sock=creerSocket(SOCK_STREAM); 
	addr=createAdr(addrIP, port);
	CHECK(bind(sock.socket, (struct sockaddr *) &addr, sizeof addr) , "Can't bind" ); 
	CHECK(listen(sock.socket, maxClts) , "Can't calibrate" );
	return sock; 
}

/********************************************************************************************************************************************************************************************************
	fonction	:	sock_t creerSocketConnect(char * addrIP, short port)
	brief		: 	Création d'une socket qui réalise une demande de connection
	param		: 	addrIP = adresse IP de l'interface utilisée 
				port = port
	result		: 	socket créée
********************************************************************************************************************************************************************************************************/
sock_t creerSocketConnect(char * addrIP, short port)
{
	sock_t sock; 
	struct sockaddr_in addr; 	
	
	// Création de la socket d’appel et de dialogue
	sock = creerSocket(SOCK_STREAM); 
	addr = createAdr(addrIP, port);  
	// Demande d’une connexion au service
	CHECK(connect(sock.socket, (struct sockaddr *)&addr, sizeof addr) , "Can't connect");
	return sock; 
}

/********************************************************************************************************************************************************************************************************
	fonction	:	int creerSocketConnect(char * addrIP, short port); 
	brief		: 	Création d'une d'échange
	param		: 	socketEchange = numéro de pipe de la socket d'écoute 
				addrIP = adresse IP de l'interface utilisée 
				port = port
				*clt = adresse client 
	result		: 	socket créée
********************************************************************************************************************************************************************************************************/
sock_t creerSocketEchange(sock_t sockEcoute, char * addrIP, short port, struct sockaddr_in *clt)
{
	int lenAddr; 
	sock_t sock; 
	
	lenAddr = sizeof(clt); 
	sock.mode = SOCK_STREAM; 
	CHECK(sock.socket=accept(sockEcoute.socket, (struct sockaddr *)&clt, &lenAddr) , "Can't connect sd");
	return sock; 
}

/********************************************************************************************************************************************************************************************************
	fonction	:	void envoyerMessSTREAM(int sock, char * mess);  
	brief		: 	Envoi d'un message en mode STREAM
	param		: 	sock = socket d'échange à utiliser pour l'envoi
				mess = Contenu du message à envoyer  
	result		: 	
********************************************************************************************************************************************************************************************************/
void envoyerMessSTREAM(sock_t sock, char * mess)
{
	CHECK(write(sock.socket, mess, strlen(mess)+1), "Can't send");
}
/********************************************************************************************************************************************************************************************************
	fonction	:	void recevoirMessSTREAM(sock_t sock, char * mess);  
	brief		: 	Reception d'un message en mode STREAM
	param		: 	sock = socket d'échange à utiliser pour l'envoi
				mess = Contenu du message à envoyer  
	result		: 	
********************************************************************************************************************************************************************************************************/
void recevoirMessSTREAM(sock_t sock, char *msg)
{
	CHECK(read(sock.socket, msg, MAX_BUFF), "Can't read");
}

//------------------------------------------------------------------------------ Cas DGRAM ---------------------------------------------------------------------------------------------------------//
/********************************************************************************************************************************************************************************************************
	fonction	:	void recevoirMessDGRAM(sock_t sock, char *msg)
	brief		: 	Reception d'un message en mode INET
	param		: 	sock = socket d'échange à utiliser pour l'envoi
				msg = Contenu du message receptionné
	result		: 	
********************************************************************************************************************************************************************************************************/
void recevoirMessDGRAM(sock_t sock, char *msg) {
    struct sockaddr_in addr;
    
    socklen_t lenAddr = sizeof(struct sockaddr_in);
    CHECK(recvfrom(sock.socket, msg, MAX_BUFF, 0, (struct sockaddr *)&addr, &lenAddr), "Can't receive");
}
/********************************************************************************************************************************************************************************************************
	fonction	:	void envoyerMessDGRAM(sock_t sock, char *msg)
	brief		: 	Envoi d'un message en mode INET
	param		: 	sock = socket d'échange à utiliser pour l'envoi
				buff = Contenu du message envoyé 
	result		: 	
********************************************************************************************************************************************************************************************************/
void envoyerMessDGRAM(sock_t sock, char *msg)
{
	struct sockaddr_in addrSrv = sock.addrSrv; 

	CHECK(sendto(sock.socket, msg, strlen(msg)+1, 0, (struct sockaddr *)&addrSrv, sizeof addrSrv), "Can't send");     
}
