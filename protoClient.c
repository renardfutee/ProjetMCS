#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#include "reqRep.h"
#include "data.h"

#define NB_REP_MAX 30

#define RESET   "\x1b[0m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define WHITE   "\x1b[37m"

typedef struct {
    char chaine[MAX_BUFF];
    int score;
} joueurRep_t;


void dialogueSrv (sock_t sd, struct sockaddr_in *srv, char *username);
void afficherPartie(int nbManche,int nbReponse , char *joueur, char *adv, char *theme, joueurRep_t* tableau); 
void jouerManche(sock_t sock, char *username, char *adv, int idPartie, int numManche); 
void afficherParties(char *parties); 

int main(int argc, char *argv[]){
	char username[MAX_BUFF];
    	struct sockaddr_in svc, clt;
	socklen_t cltLen;
	sock_t se; 
		
	if(argc < 2) {
        	printf("Veuillez vous identifier : "); 
        	scanf("%s", username);
	}
	else
        sscanf(argv[1], "%s", username);
    
	// Création de la socket de connection 
	se = creerSocketConnect("127.0.0.1", 5000); 
	// Dialogue avec le serveur
	dialogueSrv (se, &svc, username);
	fermerSocket(se);
}

void dialogueSrv (sock_t sd, struct sockaddr_in *srv, char *username) {
	int i, choix, result, compteurReponses = 0, idPartie; 
	char reponse, c;    
	char adv[MAX_BUFF], theme[MAX_BUFF], buff[MAX_BUFF];  
	reqSimple_t requete; 
	joueurRep_t* reponses = malloc(NB_REP_MAX * sizeof(joueurRep_t));
	rep_t reponseServeur; 
	
    	if (reponses == NULL) {
        	fprintf(stderr, "Erreur d'allocation de mémoire\n");
        	return 1; // Quitter le programme avec un code d'erreur
    	}

	// L'utilisateur commence par se connecter ! 
	sendRequete(sd, CONNECT, username, NULL, NULL, NULL);  
	receiveReponse(sd);
	
	do{
		printf("<--------------- MENU --------------->\n"); 
		printf("0 - Quitter le jeu\n"); 
		printf("1 - Commencer une nouvelle partie\n"); 
		printf("2 - Mon tour\n"); 
		printf("3 - Leur tour\n"); 
		scanf("%d", &choix); 
		
		switch(choix){
			case 0: 
				printf("Vous avez choisit de vous déconnecter\n"); 
				sendRequete(sd, DISCONNECT, username, NULL, NULL, NULL);
				receiveReponse(sd);
			break; 
			case 1:
				printf("Vous avez choisit de lancer une nouvelle partie\n\n Voici les joueurs disponibles : \n -----------------------------------------\n"); 
				sendRequete(sd, GET_USRS, username, NULL, NULL, NULL);
				receiveReponse(sd);
				printf("-----------------------------------------\n"); 
				
				do {
					printf("Veuillez entrer le nom du joueur que vous voulez défier: "); 
					scanf("%s", adv);
					sendRequete(sd, CHECK_PSEUDO, adv, NULL, NULL, NULL); 
					result = receiveReponse(sd).nb; 
				}while(result == 0); 
				
				sendRequete(sd, NOUV_PART, username, adv, NULL, NULL);
				printf("Id partie: "); 
				idPartie = atoi(receiveReponse(sd).msg); 
				afficherPartie(1, 0, username, adv, NULL, NULL); 
				scanf(" %c", &reponse);

				if (reponse == 'y' || reponse == 'Y') 
        				jouerManche(sd, username, adv, idPartie, result); 				
			break; 
			case 2:
				sendRequete(sd, GET_PARTIE_MON_TOUR, username, NULL, NULL, NULL);
				reponseServeur = receiveReponse(sd);
				strcpy(buff, reponseServeur.msg); 
				
				if(reponseServeur.nb == 0) {
					printf("\n\n-------------------------------------------------\n|\t");
					printf(RED "AUCUNE PARTIE" RESET); 
					printf("\t\t\t|\n------------------------------------------------\n|"); 
				}
				else {
					i = 0; 
					printf("\n------------------------------------------\n");  
					
					do{
						if(buff[i] == ':')
							printf("\t");
						else if(buff[i] == '_')
							printf("  →\t"); 
						else if(buff[i] == ';')
							printf("\n------------------------------------------\n"); 
						else if(buff[i] != ' ') 
							printf("%c", buff[i]); 
						i = i + 1; 
					}while(buff[i] != '\0'); 
					
					printf("\nChoisir un numéro pour continuer la partie (choisir 0 pour ne pas jouer) : "); 
					do{
						printf("-->");
						scanf("%d", &idPartie); 
						
						if(idPartie != 0) {
							sprintf(buff, "%d", idPartie);
							sendRequete(sd, VERIF_JOUER, username, buff, NULL, NULL);
							result = receiveReponse(sd).nb;  
						}
						else
							result = -1; 
					}while(result <= 0 && idPartie != 0);
					printf("Ok partie libre\n"); 
					
					if(idPartie != 0){
						sendRequete(sd, GET_ADV, username, buff, NULL, NULL);
						strcpy(adv, receiveReponse(sd).msg); 
						sendRequete(sd, GET_PROC_MANCHE, buff, NULL, NULL, NULL);
						result = receiveReponse(sd).nb; 
						jouerManche(sd, username, adv, idPartie, result);  
					}
					
					
				}
				printf("\n");
				
			break;
			case 3:
				sendRequete(sd, GET_PARTIE_LEUR_TOUR, username, NULL, NULL, NULL);  
				reponseServeur = receiveReponse(sd); 
				afficherParties(reponseServeur.msg); 
			break; 
			default:  
			break ; 
		}
	} while(choix != 0); 
}

void afficherPartie(int nbManche,int nbReponse , char *joueur, char *adv, char *theme, joueurRep_t* tableau)
{
	int i, j;
	char thm[MAX_BUFF];  

	printf("\n\n-------------------------------------------------\n|\t");
	
	if(theme != NULL)
		printf(CYAN "PARTIE %s contre %s" RESET , joueur, adv); 
	
	printf("\t\t|\n-------------------------------------------------\n");
	
	if (theme == NULL)
		printf("|\t\t\t\t\t\t|\n|\t\tManche %d\t\t\t|\n|\tVoulez-vous jouer cette manche ?\t|\n|\t\t\t\t\t\t|\n-------------------------------------------------\n[Y|N]--->", nbManche);
	else {
		for(j=1; j<nbManche; j++){
			printf(BLUE "|\t\tManche %d\t\t\t|" RESET, j);
			printf("\n|\t\t\t\t\t\t|\n-------------------------------------------------\n");
		}
		
		printf("|\t\t\t\t\t\t|\n");
		printf(CYAN "|\t\tManche %d\t\t\t|\n|\tTheme : %s\t\t|" RESET, nbManche, theme);
		printf("\n|\t\t\t\t\t\t|\n-------------------------------------------------\n");

		for(i=0; i<nbReponse; i++){
			printf("|\t%s \t\t\t\t", tableau[i].chaine);
			
			if( tableau[i].score == 1)
				printf(YELLOW "%d" RESET , tableau[i].score);
			else if( tableau[i].score == 2)
				printf(GREEN "%d" RESET , tableau[i].score);	
			else if( tableau[i].score == 3)
				printf(MAGENTA "%d" RESET , tableau[i].score);
			else
				printf("%d", tableau[i].score); 
				
			printf("\t|\n");
		}
		
		printf("-------------------------------------------------\n");
	}
		 
}

void jouerManche(sock_t sock, char *username, char *adv, int idPartie, int numManche)
{
	int compteurReponses = 0, result, score1, score2; 
	char theme[MAX_BUFF], buff[MAX_BUFF]; 
	rep_t reponseServeur; 
	joueurRep_t* reponses = malloc(NB_REP_MAX * sizeof(joueurRep_t));
	sendRequete(sock, LANCER_MANCHE, username, idPartie, numManche, NULL);
	strcpy(theme,receiveReponse(sock).msg);

        do {
	        afficherPartie(numManche, compteurReponses, username, adv, theme, reponses); 
        	printf("--->");
		scanf("%s", buff);
		sendRequete(sock, REPONDRE, username, buff, idPartie, numManche);
        	reponseServeur = receiveReponse(sock);
        	result = reponseServeur.nb; 
        					
        	if(reponseServeur.msg != " "){
        		strcpy(reponses[compteurReponses].chaine, buff); 
        		reponses[compteurReponses].score = atoi(reponseServeur.msg); 
        		compteurReponses = compteurReponses + 1; 
        	} 
        }while(result != 0); 
       
     	reponseServeur = receiveReponse(sock);
        afficherPartie(numManche, compteurReponses, username, adv, theme, reponses); 
        printf(RED "\tFIN DE LA MANCHE\t Score: "); 
        sendRequete(sock, GET_SCORE, username ,idPartie , numManche, NULL); 
        receiveReponse(sock);
        printf(RESET "-------------------------------------------------\n");
        printf("\n"); 
        sprintf(buff, "%d", idPartie);
  	sendRequete(sock, CHANGER_TOUR, adv,buff , NULL, NULL);
  	reponseServeur = receiveReponse(sock);
<<<<<<< HEAD

  	if(reponseServeur.nb == 0){
  		printf("Fin de la partie\n");
  		sprintf(buff, "%d", idPartie);
  		sendRequete(sock, GET_SCORE_FINAL, username, buff, NULL, NULL); 
  		 printf("-------------------------------------------------\n");
  		 printf("|\t\tSCORE FINAL: "); 
  		 reponseServeur = receiveReponse(sock);
  		 sscanf(reponseServeur.msg,"%i-%i",&score1, &score2);
  		
  		if(score1 > score2)
  			printf(GREEN "|\t\tVICTOIRE !!!!\t\t\t|\n" RESET); 
  		else if(score1 == score2)
  			printf(BLUE "|\t\tEGALITE\t\t\t|\n" RESET); 
  		else
  			printf(RED "|\t\tDEFAITE...\t\t\t|\n" RESET); 
  		
  		 printf("-------------------------------------------------\n"); 
  	}
}

void afficherParties(char *parties)
{
	int i = 0; 
	printf("\n------------------------------------------\n");  
					
	do{
		if(parties[i] == ':')
			printf("\t");
		else if(parties[i] == '_')
			printf("  →\t"); 
		else if(parties[i] == ';')
			printf("\n------------------------------------------\n"); 
		else if(parties[i] != ' ') 
			printf("%c", parties[i]); 
	
		i = i + 1; 
	}while(parties[i] != '\0'); 
	
}
=======
  	 
}
>>>>>>> e81fe161a3ea2552f65b156d39b55bbc96d99370
