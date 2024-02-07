// // connexion() recherche le pseudo dans le json si n'est pas le créer (minimum 3 lettres) sinon le connecte(renvoie 1) dans le json jsons/pseudos.json
// qui est sous la forme :

// {
//   "pseudos": ["bastian", "alexis", "lucie", "antoine", "rania"]
// }
// findRandomTheme() selectionne un theme au pif et surtout vérifie qu'il n'est pas déja utilisé donc deux fichiers json avec un thele

// 
// pseudo
// reponsesthemes
// match
//

/**
 * @brief 
 * 
 * @param pseudo 
 * @return int 
 */
int connexion(const char * pseudo);

/**
 * @brief 
 * 
 * @return char 
 */
char* findRandomTheme();

/**
 * @brief 
 * 
 * @param connected_pseudo 
 * @param recherchePseudo 
 * @return int
 */
int creategame(const char* connected_pseudo, const char* recherchePseudo);

/**
 * @brief 
 * 
 * @return char* 
 */
char** fetchAllPlayers(); 

/**
 * @brief 
 * 
 * @param id_match 
 * @param id_manche 
 * @return char* 
 */
char *RecupTheme(int id_match, int id_manche);

/**
 * @brief 
 * 
 * @param connected_pseudo 
 * @param idPartie 
 * @return char* 
 */
char *getAdversaire(const char *connected_pseudo, int idPartie);

/**
 * @brief 
 * 
 * @param id_match 
 * @param id_manche 
 * @return int 
 */
int changementTour(int id_match, int id_manche);

/**
 * @brief 
 * 
 * @param connected_pseudo 
 * @param id_match 
 * @return int 
 */
int verifGame(const char* connected_pseudo, int id_match);

/**
 * @brief 
 * 
 * @param id_match 
 * @return int 
 */
int nextManche(int id_match);