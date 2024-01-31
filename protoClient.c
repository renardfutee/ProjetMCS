#include "data.h"

int main(int argc, char *argv[]){

    char username[MAX_BUFF];
    printf("Client\n"); 
		
	if(argc < 2) {
        printf("Veuillez vous identifier \n"); 
        fgets(username, MAX_BUFF, stdin);
	}else{
        sscanf(argv[1], "%s", username);
    }
    

    printf("Bienvenue %s\n", username);

	clt("127.0.0.1", 5000, "SOCK_STREAM"); 	

}