#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "reqRep.h"

void sendRequete(sock_t sd, type_req type, generic attr1)
{
	if(type == 0)
		createReq(sd, 0); 
	else if(type > 0)
		createReqSimple(sd, type, attr1);
}

void createReq(sock_t sock, int nb)
{
	char chaine[5]; 
	req_t requete; 
	
	requete.nb = nb; 
	sprintf(chaine, "%d", requete.nb);
	envoyer(sock, chaine, NULL);
}

void createReqSimple(sock_t sock, int nb, char *msg)
{
	reqSimple_t requete; 
	
	requete.nb = nb;  
	strcpy(requete.msg, msg);
	envoyer(sock, &requete, serial);
}

void sendRep(sock_t sd, rep_t reponse)
{
	printf("%d : %s\n\n", reponse.nb, reponse.msg); 
	envoyer(sd, &reponse, serial); 
}

rep_t receiveRequete(sock_t sd)
{
	int result; 
	rep_t req; 

	recevoir (sd, &req, deSerial); 
	
	switch(req.nb){
		case 0:
			printf("---> L'utilisateur quitte l'application"); 
			req.nb = -100; 
			strcpy(req.msg, "Au revoir"); 
		break; 
		case 1:
			result = connexion(req.msg);
			req.nb = result; 
			if(result == 1)
				strcpy(req.msg, "Bonjour !"); 
			else if(result == 0)
				strcpy(req.msg, "Bienvenue !");
			else
				strcpy(req.msg, "Erreur lors de la connexion"); 
		break; 
		case 2:
			printf("-----> Demande de nouvelle partie !");  
		break; 
		default:
			printf("---> Requête non reconnue reçue fin de la connexion\n");  
			exit(0); 
		break; 
	}
		
	return req; 
}

void receiveReponse(sock_t sd)
{
	rep_t req; 	
	
	recevoir(sd, &req, deSerial);
	printf("%s\n", req.msg);
	
	if(req.nb == -1)
		exit(0);  
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
