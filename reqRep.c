#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#include "reqRep.h"

void sendRequete(sock_t sd, char *typeRequete, generic attr1)
{
	reqSimple_t requete; 

	if(strcmp(typeRequete, "connect\0") == 0){
		printf("Demande de connection au serveur par l'utilisateur : '%s'!\n", (char *) attr1); 
		requete.nb = 1; 
		strcpy(requete.msg, (char *) attr1); 
	}
	
	envoyer(sd, &requete, serial); 
}

reqSimple_t receiveRequete(sock_t sd)
{
	reqSimple_t req; 

	recevoir (sd, &req, deSerial); 	
	
	switch(req.nb){
		case 1:
			//printf("Demande de connexion de l'utilisateur %s\n", req.msg); 
			connexion(req.msg);
		break; 
	}
		
	return req; 
}

/********************************************************************************************************************************************************************************************************
	fonction	:	void serial(generic quoi, generic buff); 
	brief		: 	Serialise une requête 
	param		: 	generic quoi = requete à sérialiser 
				generic buff = requete serialisée
	result		: 	
********************************************************************************************************************************************************************************************************/
void serial(generic quoi, generic buff) 
{
    reqSimple_t req = *(reqSimple_t *) quoi;
    
    sprintf(*(buff_t *) buff, "%d:%s", req.nb, req.msg);
}

/********************************************************************************************************************************************************************************************************
	fonction	:	void serial(generic quoi, generic buff); 
	brief		: 	Serialise une requête 
	param		: 	generic quoi = requete à sérialiser 
				generic buff = requete serialisée
	result		: 	
********************************************************************************************************************************************************************************************************/
void deSerial(generic quoi, generic buff) {
	reqSimple_t *req = (reqSimple_t *) quoi;
    
    	sscanf(*(buff_t *) buff, "%d:%[^\\0]", &req->nb, req->msg);
}
