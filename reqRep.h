#include <stdio.h>

#define MAX_SIZE 200

typedef void * generic;
typedef char buff_t[MAX_SIZE]; 

typedef struct req_t {
  char msg[MAX_SIZE];
  int nb;
} reqSimple_t;


/********************************************************************************************************************************************************************************************************
	fonction	:	void serial(generic quoi, generic buff); 
	brief		: 	Serialise une requête 
	param		: 	generic quoi = requete à sérialiser 
				generic buff = requete serialisée
	result		: 	
********************************************************************************************************************************************************************************************************/
<<<<<<< HEAD
void serial(generic quoi, generic buff); 
/********************************************************************************************************************************************************************************************************
	fonction	:	void deSerial(generic quoi, generic buff); 
	brief		: 	Deserialise une requête 
	param		: 	generic quoi = requete à desérialiser 
				generic buff = requete deserialisée
	result		: 	
********************************************************************************************************************************************************************************************************/
void deSerial(generic quoi, generic buff); 
=======
int connect(char *pseudo); 


/********************************************************************************************************************************************************************************************************
	fonction	:	void affichageJoueur();
	brief		: 	Affiche la liste des joueurs défiables
	param		: 	XXX
	result		: 	Affichage de la liste des joueurs défiables
********************************************************************************************************************************************************************************************************/
void affichageJoueur();
>>>>>>> 53e2ec8222278bfd624d2b2b2fca830e301b9dbd
