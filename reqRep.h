#include "coreApp.h"
#include "data.h"

#define MAX_SIZE 200

typedef void * generic;
typedef char buff_t[MAX_SIZE]; 

typedef struct reqSimple_t {
  char msg[MAX_SIZE];
  int nb;
} reqSimple_t;

typedef struct rep {
  int nb;
} rep;


void sendRequete(sock_t sd, char *typeRequete, generic attr1);  
reqSimple_t receiveRequete(sock_t sd); 
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
