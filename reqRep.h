#include "coreApp.h"
#include "data.h"

#define MAX_SIZE 2000
#define TEMPS 5

typedef void * generic;
typedef char buff_t[MAX_SIZE]; 

typedef enum {
	DISCONNECT, 		// 0 : 0 attr 							→ type req_t
    	CONNECT,		// 1 : 1 attr (char user)					→ type reqSimple_t
    	GET_USRS, 		// 2 : 1 attr (char user) 					→ type reqSimple_t
    	CHECK_PSEUDO,		// 3 : 1 attr (char pseudo) 					→ type reqSimple_t
    	GET_PARTIE_MON_TOUR,  	// 4 : 1 attr (char user) 					→ type reqSimple_t
    	GET_PROC_MANCHE, 	// 5 : 1 attr (int idPartie) 					→ type reqSimple_t
    	GET_PARTIE_LEUR_TOUR, 	// 6 : 1 attr (char user)					→ type reqSimple_t
    	NOUV_PART, 		// 7 : 2 attr (char user, char adversaire) 			→ type reqComplete_t
    	CHANGER_TOUR, 		// 8 : 2 attr (char adversaire, char idPartie)  		→ type reqComplete_t
    	VERIF_JOUER, 		// 9 : 2 attr (char user, char idPartie)			→ type reqComplete_t
    	GET_ADV, 		// 10 : 2 attr (char user, int idPartie)			→ type reqSimple_t 
    	GET_SCORE_FINAL,	// 11 : 2 attr (char user, int idPartie) 			→ type reqComplete_t 
    	LANCER_MANCHE, 	// 12 : 3 attr (char user, int idPartie, int idManche) 	→ type reqComplexe_t
    	GET_SCORE, 		// 13 : 3 attr (char pseudo, idPartie, idManche) 		→ type reqComplexe_t
    	REPONDRE		// 14 : 4 attr (char user, char reponse, idPartie, idManche)	→ type reqReponse_t
} type_req;

/*----------------------------------------------------------------------------TYPE DE REQUETE-------------------------------------------------------------------------------------------------------*/
typedef struct req_t {
  	int nb;
} req_t;

typedef struct reqSimple_t {
  	char msg[MAX_SIZE];
  	int nb;
} reqSimple_t;


typedef struct reqComplete_t {
  	char msg[MAX_SIZE];
  	char msg2[MAX_SIZE];
  	int nb;
} reqComplete_t;

typedef struct reqComplexe_t {
  	char msg[MAX_SIZE];
  	int id1; 
  	int id2; 
  	int nb;
} reqComplexe_t;

typedef struct reqReponse_t {
  	char user[MAX_SIZE];
  	char reponse[MAX_SIZE];
  	int idPartie; 
  	int idManche; 
  	int nb;
} reqReponse_t;

/*----------------------------------------------------------------------------TYPE REPONSE-------------------------------------------------------------------------------------------------------*/

typedef struct rep {
	char msg[MAX_SIZE];
  	int nb;
} rep_t;

/*==================================================================================================================================================================================================*/

/*--------------------------------------------------------------------------Creation des requêtes----------------------------------------------------------------------------------------------------*/
/******************************************************************************************************************************************************************************************************
	fonction	:	void createReq(sock_t sock, int nb); 
	brief		: 	Crée les requêtes de type req_t
	param		: 	sock_t sock = socket
				int nb = type de la requete
	result		: 	
********************************************************************************************************************************************************************************************************/
void createReq(sock_t sock, int nb); 
/******************************************************************************************************************************************************************************************************
	fonction	:	void createReqSimple(sock_t sock, int nb, char *msg); 
	brief		: 	Crée les requêtes de type reqSimple_t
	param		: 	sock_t sock = socket
				int nb = type de la requete
				char *msg = contenu de l'attribut message de la requete
	result		: 	
********************************************************************************************************************************************************************************************************/
void createReqSimple(sock_t sock, int nb, char *msg); 
/******************************************************************************************************************************************************************************************************
	fonction	:	void createReqComptele(sock_t sock, int nb, char *msg, char *msg2);  
	brief		: 	Crée les requêtes de type reqComplete_t
	param		: 	sock_t sock = socket
				int nb = type de la requete
				char *msg = contenu de l'attribut message de la requete
				char *msg2 = contenu de l'attribut message 2 de la requete
	result		: 	
********************************************************************************************************************************************************************************************************/
void createReqComptele(sock_t sock, int nb, char *msg, char *msg2); 
/******************************************************************************************************************************************************************************************************
	fonction	:	void createReqComplexe(sock_t sock, int nb, char *msg,int nb1, int nb2); 
	brief		: 	Crée les requêtes de type reqComplexe_t
	param		: 	sock_t sock = socket
				int nb = type de la requete
				char *msg = contenu de l'attribut message de la requete
				int nb1 = id1 de la requête
				int nb2 = id2 de la requête 
	result		: 	
********************************************************************************************************************************************************************************************************/
void createReqComplexe(sock_t sock, int nb, char *msg,int nb1, int nb2); 
/******************************************************************************************************************************************************************************************************
	fonction	:	void createReqRep(sock_t sock,int nb, char *joueur, char *reponse, int idPartie, int idMatch); 
	brief		: 	Crée les requêtes de type reqReponse_t
	param		: 	sock_t sock = socket
				int nb = type de la requete
				char *joueur = pseudo du client qui envoie la requete 
				char *reponse = reponse du client
				int idPartie = identifiant de la partie en cours
				int idMatch = identifiant de la manche en cours 
	result		: 	
********************************************************************************************************************************************************************************************************/
void createReqRep(sock_t sock,int nb, char *joueur, char *reponse, int idPartie, int idMatch); 

/*--------------------------------------------------------------------------Envoi req/rep --------------------------------------------------------------------------------------------------------*/
/******************************************************************************************************************************************************************************************************
	fonction	:	void sendRequete(sock_t sd, type_req type, generic attr1, generic attr2,  generic attr3, generic attr4); 
	brief		: 	Génére la requête en fonction de son type et l'envoie au serveur
	param		: 	sock_t sock = socket
				type_req type = type de la requete
				generic attr1 = attribut 1 de la requête
				generic attr2 = attribut 2 de la requête
				generic attr3 = attribut 3 de la requête
				generic attr4 = attribut 4 de la requête
	result		: 	
********************************************************************************************************************************************************************************************************/
void sendRequete(sock_t sd, type_req type, generic attr1, generic attr2,  generic attr3, generic attr4); 
void sendRep(sock_t sd, rep_t reponse);  

/*--------------------------------------------------------------------------Reception req/rep ------------------------------------------------------------------------------------------------------*/
/******************************************************************************************************************************************************************************************************
	fonction	:	rep_t receiveRequete(sock_t sd); 
	brief		: 	Receptionne les requêtes et execute le traitement correspondant
	param		: 	sock_t sock = socket
	result		: 	Retourne la reponse du serveur à la requête 
********************************************************************************************************************************************************************************************************/
rep_t receiveRequete(sock_t sd); 
/******************************************************************************************************************************************************************************************************
	fonction	:	rep_t receiveReponse(sock_t sd); 
	brief		: 	Receptionne la réponse du serveur pour le client 
	param		: 	sock_t sock = socket
	result		: 	Retourne la reponse du serveur
********************************************************************************************************************************************************************************************************/
rep_t receiveReponse(sock_t sd); 

/*--------------------------------------------------------------------------Traitements de requête ------------------------------------------------------------------------------------------------*/
/******************************************************************************************************************************************************************************************************
	fonction	:	void deconnecter(rep_t * rep); 
	brief		: 	Deconnecter le joueur
	param		: 	rep_t *rep = reponse à envoyer au client
	result		: 	
********************************************************************************************************************************************************************************************************/
void deconnecter(rep_t * rep); 
/******************************************************************************************************************************************************************************************************
	fonction	:	void connecter(rep_t * rep, char * pseudo);
	brief		: 	Connecter le joueur
	param		: 	rep_t *rep = reponse à envoyer au client
				char *pseudo = pseudo du joueur à connecter
	result		: 	
********************************************************************************************************************************************************************************************************/
void connecter(rep_t * rep, char * pseudo); 
/******************************************************************************************************************************************************************************************************
	fonction	:	void getUsers(rep_t * rep, char * pseudo); 
	brief		: 	Récupère la liste des joueurs sauf le joueurs connecté
	param		: 	rep_t *rep = reponse à envoyer au client
				char *pseudo = pseudo du joueur à connecter
	result		: 	
********************************************************************************************************************************************************************************************************/
void getUsers(rep_t * rep, char * pseudo); 
/******************************************************************************************************************************************************************************************************
	fonction	:	void verifUser(rep_t * rep, char * adv); 
	brief		: 	Vérifie que l'adversaire choisit par le client existe
	param		: 	rep_t *rep = reponse à envoyer au client
				char *adv = adversaire choisit par le client 
	result		: 	
********************************************************************************************************************************************************************************************************/
void verifUser(rep_t * rep, char * adv); 
/******************************************************************************************************************************************************************************************************
	fonction	:	void changerTour(rep_t * rep, reqSimple_t req); 
	brief		: 	Change le tour de joueur pour une partie
	param		: 	rep_t *rep = reponse à envoyer au client
				reqSimple_t req = Requête reçue
	result		: 	
********************************************************************************************************************************************************************************************************/
void changerTour(rep_t * rep, reqSimple_t req); 
/******************************************************************************************************************************************************************************************************
	fonction	:	void lancerPartie(rep_t * rep, reqSimple_t req); 
	brief		: 	Lancer une partie
	param		: 	rep_t *rep = reponse à envoyer au client
				reqSimple_t req = Requête reçue
	result		: 	
********************************************************************************************************************************************************************************************************/
void lancerPartie(rep_t * rep, reqSimple_t req);
/******************************************************************************************************************************************************************************************************
	fonction	:	void verif_jouer(rep_t * rep, reqSimple_t req); 
	brief		: 	Vérifier qu'un joueur peut joueur une partie
	param		: 	rep_t *rep = reponse à envoyer au client
				reqSimple_t req = Requête reçue
	result		: 	
********************************************************************************************************************************************************************************************************/
void verif_jouer(rep_t * rep, reqSimple_t req);
/******************************************************************************************************************************************************************************************************
	fonction	:	void getAdv(rep_t * rep, reqSimple_t req);  
	brief		: 	Récupère le pseudo de l'adversaire d'un joueur sur une partie
	param		: 	rep_t *rep = reponse à envoyer au client
				reqSimple_t req = Requête reçue
	result		: 	
********************************************************************************************************************************************************************************************************/
void getAdv(rep_t * rep, reqSimple_t req);  
/******************************************************************************************************************************************************************************************************
	fonction	:	void getFinalScore(rep_t * rep, reqSimple_t req);
	brief		: 	Récupère le score final d'une partie
	param		: 	rep_t *rep = reponse à envoyer au client
				reqSimple_t req = Requête reçue
	result		: 	
********************************************************************************************************************************************************************************************************/ 
void getFinalScore(rep_t * rep, reqSimple_t req);
/******************************************************************************************************************************************************************************************************
	fonction	:	void lancerManche(rep_t * rep, reqSimple_t req); 
	brief		: 	Commence une manche d'une partie
	param		: 	rep_t *rep = reponse à envoyer au client
				reqSimple_t req = Requête reçue
	result		: 	
********************************************************************************************************************************************************************************************************/ 
void lancerManche(rep_t * rep, reqSimple_t req); 
/******************************************************************************************************************************************************************************************************
	fonction	:	void getScore(rep_t * rep, reqSimple_t req);
	brief		: 	Récupère le score d'une manche
	param		: 	rep_t *rep = reponse à envoyer au client
				reqSimple_t req = Requête reçue
	result		: 	
********************************************************************************************************************************************************************************************************/ 
void getScore(rep_t * rep, reqSimple_t req);
/******************************************************************************************************************************************************************************************************
	fonction	:	void getReponse(rep_t * rep, reqSimple_t req); 
	brief		: 	Evalue la réponse d'un joueur
	param		: 	rep_t *rep = reponse à envoyer au client
				reqSimple_t req = Requête reçue
	result		: 	
********************************************************************************************************************************************************************************************************/ 
void getReponse(rep_t * rep, reqSimple_t req); 

/*--------------------------------------------------------------------------Serialisation --------------------------------------------------------------------------------------------------------*/
/********************************************************************************************************************************************************************************************************
	fonction	:	void serial(generic quoi, generic buff); 
	brief		: 	Serialise une requête 
	param		: 	generic quoi = requete à sérialiser 
				generic buff = requete serialisée
	result		: 	
********************************************************************************************************************************************************************************************************/
void serial(generic quoi, generic buff); 

void serialComplete(generic quoi, generic buff); 
void serialComplexe(generic quoi, generic buff); 
void serialRep(generic quoi, generic buff);

/*--------------------------------------------------------------------------Deserialisation --------------------------------------------------------------------------------------------------------*/
 
/********************************************************************************************************************************************************************************************************
	fonction	:	void deSerial(generic quoi, generic buff); 
	brief		: 	Deserialise une requête 
	param		: 	generic quoi = requete à desérialiser 
				generic buff = requete deserialisée
	result		: 	
********************************************************************************************************************************************************************************************************/
void deSerial(generic quoi, generic buff); 

void deSerialComplete(generic quoi, generic buff); 
void deSerialComplexe(generic quoi, generic reqSimple); 
void deSerialReponse(generic quoi, generic reqSimple); 
