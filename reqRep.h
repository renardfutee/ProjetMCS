#include "coreApp.h"
#include "data.h"

#define MAX_SIZE 2000
#define TEMPS 15

typedef void * generic;
typedef char buff_t[MAX_SIZE]; 

typedef enum {
	DISCONNECT, 		// 0 : 0 attr → type req_t
    	CONNECT,		// 1 : 1 attr → type reqSimple_t
    	GET_USRS, 		// 2 : 1 attr → type reqSimple_t
    	CHECK_PSEUDO,		// 3 : 1 attr → type reqSimple_t
    	GET_PARTIE_MON_TOUR,  	// 4 : 1 attr → type reqSimple_t
    	GET_PROC_MANCHE, 	// 5 : 1 attr → type reqSimple_t
    	NOUV_PART, 		// 6 : 2 attr → type reqComplete_t
    	CHANGER_TOUR, 		// 7 : 2 (char adversaire, char idPartie)  attr		→ type reqComplete_t
    	VERIF_JOUER, 		// 8 : 2 attr (char user, char idPartie)			→ type reqComplexe_t
    	GET_ADV, 		// 9 : 2 attr → type reqSimple_t 
    	LANCER_MANCHE, 	// 10 : 3 attr → type reqComplexe_t
    	GET_SCORE, 		// 11 : 3 attr (char pseudo, idPartie, idManche) 		→ type reqComplexe_t
    	REPONDRE		// 12 : 4 attr (char user, char reponse, idPartie, idManche)	→ type reqReponse_t
} type_req;


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

typedef struct rep {
	char msg[MAX_SIZE];
  	int nb;
} rep_t;


void createReq(sock_t sock, int nb); 
void createReqSimple(sock_t sock, int nb, char *msg); 
void createReqComptele(sock_t sock, int nb, char *msg, char *msg2); 
void createReqComplexe(sock_t sock, int nb, char *msg,int nb1, int nb2); 
void createReqRep(sock_t sock,int nb, char *joueur, char *reponse, int idPartie, int idMatch); 
void sendRequete(sock_t sd, type_req type, generic attr1, generic attr2,  generic attr3, generic attr4); 
void sendRep(sock_t sd, rep_t reponse);  
rep_t receiveRequete(sock_t sd); 
rep_t receiveReponse(sock_t sd); 

void deconnecter(rep_t * rep); 
void connecter(rep_t * rep, char * pseudo); 
void getUsers(rep_t * rep, char * pseudo); 
void verifUser(rep_t * rep, char * adv); 
void changerTour(rep_t * rep, reqSimple_t req); 
void lancerPartie(rep_t * rep, reqSimple_t req);
void verif_jouer(rep_t * rep, reqSimple_t req);
void getAdv(rep_t * rep, reqSimple_t req);   
void lancerManche(rep_t * rep, reqSimple_t req); 
void getScore(rep_t * rep, reqSimple_t req);
void getReponse(rep_t * rep, reqSimple_t req); 
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
