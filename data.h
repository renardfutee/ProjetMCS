#include "session.h"

#define MAX_BUFF 1024

typedef char buffer_t[MAX_BUFF]; 
typedef void * generic;
typedef void (*pfct)(generic, generic); 

/********************************************************************************************************************************************************************************************************
	fonction	:	void envoyer(int sock, generic quoi, pfct serrial); 
	brief		: 	Envoi d'une requête/réponse sur une socket 
	param		: 	sock = socket d'échange à utiliser pour l'envoi
				quoi = requete/reponse à serialiser avant l'envoi 
				serial = pointeur sur la fonction de serialisation d'une requête/reponse 
	result		: 	
********************************************************************************************************************************************************************************************************/
void envoyer(sock_t sock, generic quoi, pfct serial, ...); 
/********************************************************************************************************************************************************************************************************
	fonction	:	void recevoir (int sock, generic quoi, pfct deSerial); 
	brief		: 	Reception d'une requête/reponse sur une socket 
	param		: 	sock = socket d'échange à utiliser pour la reception
				quoi = requete/reponse reçue après deseriallisation du buffer de réception 
				deSerial = pointeur sur la fonction de deserialisation d'une requête/reponse 
	result		: 	
********************************************************************************************************************************************************************************************************/
void recevoir (sock_t sock, generic quoi, pfct deSerial); 
/********************************************************************************************************************************************************************************************************
	fonction	:	void serial(generic quoi, generic buff); 
	brief		: 	Serialise une requête 
	param		: 	generic quoi = requete à sérialiser 
				generic buff = requete serialisée
	result		: 	
********************************************************************************************************************************************************************************************************/
void serial(generic quoi, generic buff); 
/********************************************************************************************************************************************************************************************************
	fonction	:	void deSerial(generic quoi, generic buff); 
	brief		: 	Deserialise une requête 
	param		: 	generic quoi = requete à desérialiser 
				generic buff = requete deserialisée
	result		: 	
********************************************************************************************************************************************************************************************************/
void deSerial(generic quoi, generic buff); 
