#include "reqRep.h"

/********************************************************************************************************************************************************************************************************
	fonction	:	void serial(generic quoi, generic buff); 
	brief		: 	Serialise une requête 
	param		: 	generic quoi = requete à sérialiser 
				generic buff = requete serialisée
	result		: 	
********************************************************************************************************************************************************************************************************/
void serial(generic quoi, generic buff) 
{
    req_t req = *(req_t *) quoi;
    
    sprintf(*(buff_t *) buff, "%d:%s", req.nb, req.msg);
}

/********************************************************************************************************************************************************************************************************
<<<<<<< HEAD
	fonction	:	void deSerial(generic quoi, generic buff); 
	brief		: 	Deserialise une requête 
	param		: 	generic quoi = requete à desérialiser 
				generic buff = requete deserialisée
	result		: 	
********************************************************************************************************************************************************************************************************/
void deSerial(generic quoi, generic buff) {
	req_t *req = (req_t *) quoi;
=======
	fonction	:	int connect(char *pseudo); 
	brief		: 	Connecte ou crée l'utilisateur depuis son pseudo 
	param		: 	pseudo = pseudo de connection de l'utilisateur 
	result		: 	retourn 1 si un nouel utilisateur a été créé, 0 si l'utilisateur existait et -1 en cas d'erreur 
********************************************************************************************************************************************************************************************************/
void affichageJoueur(){
>>>>>>> 53e2ec8222278bfd624d2b2b2fca830e301b9dbd
    
    	sscanf(*(buff_t *) buff, "%d:%[^\\0]", &req->nb, req->msg);
}
