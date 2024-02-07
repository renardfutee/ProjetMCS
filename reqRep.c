#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "reqRep.h"

void sendRequete(sock_t sd, type_req type, generic attr1, generic attr2,  generic attr3, generic attr4)
{
	if(type == 0)
		createReq(sd, type); 
	else if(type > 0 && type <= 5)
		createReqSimple(sd, type, attr1);
	else if(type <= 9)
		createReqComptele(sd, type, attr1, attr2); 
	else if(type <= 11)
		createReqComplexe(sd,type, attr1, attr2, attr3); 
	else 
		createReqRep(sd,type, attr1, attr2, attr3, attr4); 
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

void createReqComplexe(sock_t sock, int nb, char *msg,int nb1, int nb2)
{
	reqComplexe_t requete; 
	buff_t buffer; 
	
	requete.nb = nb; 
	requete.id1 = nb1; 
	requete.id2 = nb2; 
	strcpy(requete.msg, msg); 
	envoyer(sock, &requete, serialComplexe);
}

void createReqRep(sock_t sock,int nb, char *joueur, char *reponse, int idPartie, int manche)
{
	reqReponse_t requete; 
	buff_t buffer; 
	
	requete.nb = nb; 
	requete.idManche = manche; 
	requete.idPartie = idPartie; 
	strcpy(requete.reponse, reponse); 
	strcpy(requete.user, joueur); 
 	envoyer(sock, &requete, serialRep);
}

void sendRep(sock_t sd, rep_t reponse)
{
	printf("send %d -> \"%s\"\n", reponse.nb, reponse.msg); 
	envoyer(sd, &reponse, serial); 
}

rep_t receiveRequete(sock_t sd)
{
	int result; 
	reqSimple_t req; 
	rep_t rep; 

	recevoir (sd, &req, deSerial); 
	printf("NB :: %d\n", req.nb); 
	
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
			verifUser(&rep, req.msg); 
		break; 
		case 4:
			listerPartieMonTour(&rep, req); 
		break; 
		case 5:
			getProchaineManche(&rep, req); 
		break; 
		case 6:
			lancerPartie(&rep, req); 
		break;
		case 7:
			changerTour(&rep, req); 
		break; 
		case 8:
			verif_jouer(&rep, req); 
		break;
		case 9:
			getAdv(&rep, req); 
		break;  
		case 10:
			lancerManche(&rep, req);  
		break; 
		case 11:
			getScore(&rep, req); 
		break; 
		case 12:
			getReponse(&rep, req);  
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
	strcat(pseudo, "\n"); 
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
	char chaine[MAX_BUFF]; 
    	char ** joueurs = fetchAllPlayers(pseudo);
                
        strcpy(chaine, ""); 
                
        for (int i = 0; joueurs[i] != NULL; i++) {
        	strcat(chaine, joueurs[i]); 
                free(joueurs[i]);
        }
         
        free(joueurs);
        rep->nb = 1; 
        strcpy(rep->msg, chaine); 
}

void verifUser(rep_t * rep, char * adv)
{
	printf("----> Vérification du choix de l'adversaire :: %s\n", adv); 
	strcat(adv, "\n");
	rep->nb = recherchePseudo(adv);
			
	if(rep->nb == 0) 
		strcpy(rep->msg, "Ce pseudo n'existe pas"); 			
	else
		strcpy(rep->msg, "La partie peut commencer !"); 
}

void listerPartieMonTour(rep_t * rep, reqSimple_t req)
{
	char * chaine = fetchAllPartiesIfTurn(req.msg);
	printf("---> L'utilisateur '%s' souhaite lister les parties ou c'est sont tour\n", req.msg); 
	
	rep->nb = 1; 
	strcpy(rep->msg, chaine); 
}

void getProchaineManche(rep_t * rep, reqSimple_t req)
{
	int manche = nextManche(atoi(req.msg));

	printf("---> Récuperer le numéro de la prochaine manche de la partie %d\n", atoi(req.msg)); 
	printf("Prochaine manche : %d\n", nextManche(atoi(req.msg))); 
	rep->nb = manche; 
	
	if(manche == -1)
		rep->nb = 0;
		 
	strcpy(rep->msg, " "); 
}

void changerTour(rep_t * rep, reqSimple_t req)
{	
	reqComplete_t reqComplete; 

	deSerialComplete(&reqComplete, &req);

	// TODO fonction qui change le tour de la partie (met fin si c'était la dernière manche)
	
	changementTour(atoi(reqComplete.msg2), 1);
	
	rep->nb = 1; 
	strcpy(rep->msg, " "); 
}

void lancerPartie(rep_t * rep, reqSimple_t req)
{
	int idPartie; 
	char chaineId[MAX_BUFF]; 
	reqComplete_t reqComplete; 

	deSerialComplete(&reqComplete, &req);
	printf("---> Lancement de partie entre les utilisateur %s et %s\n", reqComplete.msg, reqComplete.msg2);  
	idPartie = creategame(reqComplete.msg, reqComplete.msg2);
	sprintf(chaineId, "%d", idPartie);
	rep->nb = 1; 
	strcpy(rep->msg, chaineId); 
}

void verif_jouer(rep_t * rep, reqSimple_t req)
{
	int result; 
	char chaine[MAX_BUFF]; 
	reqComplete_t reqComplete; 
    // TODO
	printf("Debut de la fonciton verif\n"); 
	deSerialComplete(&reqComplete, &req);
	printf("%d--->L'utilisateur %s souhaite jouer à la partie d'identifiant %d  ", req.nb, reqComplete.msg, atoi(reqComplete.msg2)); 
	printf("Résultat de la vérification du tour : %d\n", verifGame(reqComplete.msg, atoi(reqComplete.msg2)));
	result = verifGame(reqComplete.msg, atoi(reqComplete.msg2)); 
	rep->nb = result; 
	
	if(result < 0) {
		rep->nb = 0; 
		strcpy(rep->msg, "Vous ne pouvez pas jouer cette partie"); 
	}
	else 
		strcpy(rep->msg, " "); 
}

void getAdv(rep_t * rep, reqSimple_t req)
{
	char adv[MAX_BUFF]; 
	reqComplete_t reqComplete; 

	deSerialComplete(&reqComplete, &req);
	printf("--->L'utilisateur %s souhaite connaitre l'adversaire de la partie %d\n", reqComplete.msg, atoi(reqComplete.msg2)); 
	// TODO fonction qui récupère l'adversaire du joueur pour la partie donnée
	strcpy(adv, getAdversaire(reqComplete.msg, atoi(reqComplete.msg2)));
	rep->nb = 1; 
	strcpy(rep->msg, adv); 
	
}


void lancerManche(rep_t * rep, reqSimple_t req)
{
	char theme[MAX_BUFF]; 
	reqComplexe_t reqComp; 
	
	deSerialComplexe(&reqComp, &req);
	strcpy(theme, RecupTheme(reqComp.id1, reqComp.id2));
	printf("---> Le match est lancé !\n"); 
	alarm(TEMPS); 
	rep->nb = 1; 
	strcpy(rep->msg, theme); 
}

void getScore(rep_t * rep, reqSimple_t req)
{
	int score1, score2; 
	char chaine[MAX_BUFF]; 
	reqComplexe_t reqComp; 
	
	deSerialComplexe(&reqComp, &req);
	printf("---> L'utilisateur '%s' souhaite récuperer son score pour la manche %d de la partie %d\n", reqComp.msg, reqComp.id2, reqComp.id1); 
	// TODO récupérer le score du match de la partie (toujours le score du joueur connecté en premier, si possible mettre -1 au score du si un joueur n'a pas encore joué) 10:3
	//score = scoreGame(reqComp.msg,  reqComp.id1, reqComp.id2);
	printf("SCORE :: %s\n", scoreGame(reqComp.msg,  2, 1)); 
	sscanf("11:-1","%i:%i",&score1, &score2);
	
	if(score2 < 0){
		rep->nb = 0; 
		sprintf(chaine, "%d", score1);
		strcpy(rep->msg, chaine); 
	}
	else {
		rep->nb = 1; 
		strcpy(rep->msg, "11:-1"); 
	}
}

void getReponse(rep_t * rep, reqSimple_t req)
{
	int points; 
	reqReponse_t reqRep; 

	deSerialReponse(&reqRep, &req);
	printf("---> Verification de réponse\n");
	//TODO Modifier le score du joueur qui a joué et récuperer le score obtenu grace à CETTE réponse
	rep->nb = 1; 
	int score = mettreAJourScore(reqRep.idPartie, reqRep.idManche, reqRep.user, reqRep.reponse);
	if(score == -1){
		strcpy(rep->msg, "0");
	}
	else if(score == 1){
		printf("Score == 1\n");
		strcpy(rep->msg, "1");
	}
	else if(score == 2){
		strcpy(rep->msg, "2");
	}
	else if(score == 3){
		strcpy(rep->msg, "3");
	}
	else {
		strcpy(rep->msg, "0");
	}
	printf(" rep msg   %s", rep->msg);
	printf("\n");
	printf(" score !! -----%d ----", score);
	//strcpy(rep->msg, );
	// strcpy(rep->msg, " ");
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

void serialComplexe(generic quoi, generic buff)
{
	reqComplexe_t req = *(reqComplexe_t *) quoi;
	
	sprintf(*(buff_t *) buff, "%d:%s:%d:%d", req.nb, req.msg, req.id1, req.id2);
}

void serialRep(generic quoi, generic buff)
{
	reqReponse_t req = *(reqReponse_t *) quoi;
	
	sprintf(*(buff_t *) buff, "%d:%s:%s:%d:%d", req.nb, req.user, req.reponse, req.idPartie, req.idManche);
}
/********************************************************************************************************************************************************************************************************
	fonction	:	void serial(generic quoi, generic buff); 
	brief		: 	Serialise une requête 
	param		: 	generic quoi = requete à sérialiser 
				generic buff = requete serialisée
	result		: 	
********************************************************************************************************************************************************************************************************/
void deSerial(generic quoi, generic buff) 
{
		reqSimple_t *req = (reqSimple_t *) quoi;

    		sscanf(*(buff_t *) buff, "%d:%[^\\0]", &req->nb, req->msg);
}

void deSerialComplete(generic quoi, generic reqSimple) 
{
		buff_t buffer; 
		reqComplete_t *req = (reqComplete_t *) quoi;
		reqSimple_t *reqS = (reqSimple_t *) reqSimple;
	
		req->nb = reqS->nb; 
		strcpy(buffer, *(buff_t *) reqS->msg); 
    		sscanf(buffer, "%[^:]:%s", req->msg, req->msg2);
}

void deSerialComplexe(generic quoi, generic reqSimple) 
{
		buff_t buffer, buffTemp; 
		reqComplexe_t *req = (reqComplexe_t *) quoi;
		reqSimple_t *reqS = (reqSimple_t *) reqSimple;
	
		req->nb = reqS->nb; 
		strcpy(buffer, *(buff_t *) reqS->msg); 
		sscanf(buffer, "%[^:]:%s", req->msg, buffTemp);
		sscanf(buffTemp,"%i:%i",&req->id1, &req->id2);
}

void deSerialReponse(generic quoi, generic reqSimple) 
{
		buff_t buffer, buffTemp; 
		reqReponse_t *req = (reqReponse_t *) quoi;
		reqSimple_t *reqS = (reqSimple_t *) reqSimple;
	
		req->nb = reqS->nb; 
		strcpy(buffer, *(buff_t *) reqS->msg); 
		sscanf(buffer, "%[^:]:%s", req->user, buffTemp);
		sscanf(buffTemp, "%[^:]:%s", req->reponse, buffTemp);
		sscanf(buffTemp,"%i:%i",&req->idPartie, &req->idManche);
}
