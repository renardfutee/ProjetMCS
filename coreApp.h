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
 * @param id_match 
 * @param id_manche 
 * @param pseudo 
 * @param mot 
 * @return int 
 */
int mettreAJourScore(int id_match, int id_manche, const char *pseudo, const char *mot);

/**
 * @brief 
 * 
 * @return char 
 */
char* findRandomTheme();

/**
 * @brief Permet de créer une partie en insérant les information nécessaires pour la partie
 * 
 * @param connected_pseudo 
 * @param recherchePseudo 
 * @return int
 */
int creategame(const char* connected_pseudo, const char* recherchePseudo);

/**
 * @brief Retourne les pseudos de tous les joeurs enregistré côté serveur excepté celui qui se connecte
 * 
 * @return char* 
 */
char** fetchAllPlayers(); 

/**
 * @brief Permet de récupérer le thème d'une manche
 * 
 * @param id_match 
 * @param id_manche 
 * @return char* 
 */
char *RecupTheme(int id_match, int id_manche);

/**
 * @brief Retourne le pseudo de l'adversaire de kla partie
 * 
 * @param connected_pseudo 
 * @param idPartie 
 * @return char* 
 */
char *getAdversaire(const char *connected_pseudo, int idPartie);

/**
 * @brief Change le tour de jeu en mettant le pseudo du joueur qui doit jouer ou FINI si fin du match
 * 
 * @param id_match 
 * @param id_manche 
 * @return int 
 */
int changementTour(int id_match, int id_manche);

/**
 * @brief Permet de vérifier si c'est au tour du joueur connecté de jouer au match
 * 
 * @param connected_pseudo 
 * @param id_match 
 * @return int 
 */
int verifGame(const char* connected_pseudo, int id_match);

/**
 * @brief Retourne l'id de la manche en cours
 * 
 * @param id_match 
 * @return int 
 */
int nextManche(int id_match);

/**
 * @brief Renvoie le score d'une manche spécifique en char et non en int
 * 
 * @param connected_pseudo 
 * @param id_match 
 * @param id_manche 
 * @return char* 
 */
char *scoreGame(const char *connected_pseudo, int id_match, int id_manche);

/**
 * @brief Renvoie le score d'une partie spécifique en char et non en int
 * 
 * @param connected_pseudo 
 * @param id_match 
 * @return char* 
 */
char *scorePartie(const char *connected_pseudo, int id_match);

/**
<<<<<<< HEAD
 * @brief 
=======
 * @brief Renvoie toute les parties en cours pour un joueur connecté et fini aussi avec le pseudo de l'adversaire et le score en cours
>>>>>>> e81fe161a3ea2552f65b156d39b55bbc96d99370
 * 
 * @param connected_pseudo 
 * @return char* 
 */
char *fetchAllParties(const char *connected_pseudo);
<<<<<<< HEAD
=======

/**
 * @brief Renvoie toute les parties en cours pour un joueur connecté et fini aussi avec le pseudo de l'adversaire et le score en cours
 * 
 * @param connected_pseudo 
 * @return char* 
 */
char *fetchAllPartiesIfTurn(const char *connected_pseudo);

/**
 * @brief Renvoie toute les parties en cours pour un joueur connecté et fini aussi avec le pseudo de l'adversaire et le score en cours
 * 
 * @param connected_pseudo 
 * @return char* 
 */
char *fetchAllPartiesIfNotTurn(const char *connected_pseudo);
>>>>>>> e81fe161a3ea2552f65b156d39b55bbc96d99370
