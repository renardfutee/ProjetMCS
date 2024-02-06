#include "coreApp.h"
#include "data.h"

#define MAX_SIZE 2000
#define TEMPS 5

typedef void * generic;
typedef char buff_t[MAX_SIZE]; 

typedef enum {
	DISCONNECT, 
    	CONNECT,
    	GET_USRS, 
    	CHECK_PSEUDO, 
    	NOUV_PART, 
    	LANCER_MANCHE
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

typedef struct rep {
	char msg[MAX_SIZE];
  	int nb;
} rep_t;


void createReq(sock_t sock, int nb); 
void createReqSimple(sock_t sock, int nb, char *msg); 
void createReqComptele(sock_t sock, int nb, char *msg, char *msg2); 
void createReqComplexe(sock_t sock, int nb, char *msg,int nb1, int nb2); 
void sendRequete(sock_t sd, type_req type, generic attr1, generic attr2,  generic attr3); 
void sendRep(sock_t sd, rep_t reponse);  
rep_t receiveRequete(sock_t sd); 
rep_t receiveReponse(sock_t sd); 

void deconnecter(rep_t * rep); 
void connecter(rep_t * rep, char * pseudo); 
void getUsers(rep_t * rep, char * pseudo); 
void verifUser(rep_t * rep, char * adv); 
void lancerPartie(rep_t * rep, reqSimple_t req); 
void lancerManche(rep_t * rep, reqSimple_t req); 
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
