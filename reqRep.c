#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "reqRep.h"

void sendRequete(sock_t sd, type_req type, generic attr1, generic attr2)
{
	if(type == 0)
		createReq(sd, 0); 
	else if(type > 0 && type <= 3)
		createReqSimple(sd, type, attr1);
	else{
		createReqComptele(sd, type, attr1, attr2); 
	}
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

void createReqComptele(sock_t sock, int nb, char *msg, char *msg2)
{
	reqComplete_t requete; 
	buff_t buffer; 
	
	requete.nb = nb; 
	strcpy(requete.msg, msg); 
	strcpy(requete.msg2, msg2); 
	envoyer(sock, &requete, serialComplete);
}

void sendRep(sock_t sd, rep_t reponse)
{
	envoyer(sd, &reponse, serial); 
}

rep_t receiveRequete(sock_t sd)
{
	int result; 
	reqSimple_t req; 
	rep_t rep; 

	recevoir (sd, &req, deSerial); 
	
	switch(req.nb){
		case 0:
			deconnecter(&rep);
		break; 
		case 1:
			connecter(&rep, req.msg); 
		break; 
		case 2:
			getUsers(&rep, req.msg);
		break; 
		case 3:
			verifUser(&rep, req.msg) ; 
		break; 
		case 4:
			lancerPartie(&rep, req); 
		break; 
		default:
			printf("---> Requête non reconnue reçue fin de la connexion\n");  
			rep.nb = -1; 
			exit(0); 
		break; 
	}
		
	return rep; 
}

void deconnecter(rep_t * rep)
{
	printf("---> L'utilisateur quitte l'application\n"); 
	rep->nb = -100; 
	strcpy(rep->msg, "Au revoir"); 
}

void connecter(rep_t * rep, char * pseudo)
{
	int result = connexion(pseudo);
	rep->nb = result; 
	
	if(result == 1)
		strcpy(rep->msg, "Bonjour !"); 
	else if(result == 0)
			strcpy(rep->msg, "Bienvenue !");
	else
		strcpy(rep->msg, "Erreur lors de la connexion"); 
}

void getUsers(rep_t * rep, char * pseudo)
{
	printf("-----> Demande de nouvelle partie !\n");  
	// TODO récuerer la liste des pseudos dans la chaine de caractère (sans le pseudo de l'utilisateur) 
	strcpy(rep->msg, "Mathis\nAgathe\nNinon"); 
}

void verifUser(rep_t * rep, char * adv)
{
	printf("----> Vérification du choix de l'adversaire :: %s\n", adv); 
	// TODO FONCTION QUI VERIFIE SI LE PESUDO EST BIEN DANS LA BDD 
	rep->nb = rand() % 2;
			
	if(rep->nb == 0) 
		strcpy(rep->msg, "Ce pseudo n'existe pas"); 			
	else
		strcpy(rep->msg, "La partie peut commencer !"); 
}

void lancerPartie(rep_t * rep, reqSimple_t req)
{
	reqComplete_t reqComplete; 

	deSerialComplete(&reqComplete, &req);
	printf("---> Lancement de partie entre les utilisateur %s et %s\n", reqComplete.msg, reqComplete.msg2);  
	//creategame(reqComplete.msg, reqComplete.msg2);
	// TODO Renvoeyr le theme au lieu de rien renvoyer dans la fonction creategame (rep.nb = -1 si le theme est nul)
	rep->nb = 1; 
	strcpy(rep->msg, "theme"); 
}

rep_t receiveReponse(sock_t sd)
{
	rep_t rep; 	
	
	recevoir(sd, &rep, deSerial);
	printf("%s\n", rep.msg);
	
	if(rep.nb == -1)
		exit(0);  
		
	return rep; 
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

void serialComplete(generic quoi, generic buff)
{
	reqComplete_t req = *(reqComplete_t *) quoi;
	
	sprintf(*(buff_t *) buff, "%d:%s:%s", req.nb, req.msg, req.msg2);
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

void deSerialComplete(generic quoi, generic reqSimple) {
		buff_t buffer; 
		reqComplete_t *req = (reqComplete_t *) quoi;
		reqSimple_t *reqS = (reqSimple_t *) reqSimple;
	
		req->nb = reqS->nb; 
		strcpy(buffer, *(buff_t *) reqS->msg); 
    		sscanf(buffer, "%[^:]:%s", req->msg, req->msg2);
}
