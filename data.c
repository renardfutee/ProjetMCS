#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "data.h"

/********************************************************************************************************************************************************************************************************
	fonction	:	void envoyer(int sock, generic quoi, pfct serrial); 
	brief		: 	Envoi d'une requête/réponse sur une socket 
	param		: 	sock = socket d'échange à utiliser pour l'envoi
				quoi = requete/reponse à serialiser avant l'envoi 
				serial = pointeur sur la fonction de serialisation d'une requête/reponse 
	result		: 	
********************************************************************************************************************************************************************************************************/
void envoyer(sock_t sock, generic quoi, pfct serial, ...)
{
    	va_list ap;
    	buffer_t buff;
    
    	if(sock.mode == SOCK_STREAM){
        	if(serial == NULL)
            		envoyerMessSTREAM(sock, (char *) quoi); 
        	else { 
        		serial(quoi,buff);
        		envoyerMessSTREAM(sock,buff);
        	}
    	}
    	else {
	        if(serial == NULL)
            		envoyerMessDGRAM(sock, (char *) quoi); 
        	else { 
        		serial(quoi,buff);
        		envoyerMessDGRAM(sock,buff);
        	}
    	}
}

/********************************************************************************************************************************************************************************************************
	fonction	:	void recevoir (int sock, generic quoi, pfct deSerial); 
	brief		: 	Reception d'une requête/reponse sur une socket 
	param		: 	sock = socket d'échange à utiliser pour la reception
				quoi = requete/reponse reçue après deseriallisation du buffer de réception 
				deSerial = pointeur sur la fonction de deserialisation d'une requête/reponse 
	result		: 	
********************************************************************************************************************************************************************************************************/
void recevoir (sock_t sock, generic quoi, pfct deSerial)
{		
    	buffer_t buff;
    	struct sockaddr_in addr;

    	if (sock.mode == SOCK_STREAM) { 
        	if (deSerial == NULL) 
        		recevoirMessSTREAM(sock, (char *)quoi);
        	else {
        	    	recevoirMessSTREAM(sock, (char *)buff);
        	    	deSerial(quoi, buff);
        	}
    	} 
    	else{ 
    		if(deSerial == NULL)
    			recevoirMessDGRAM(sock, (char *) buff); 
    		else{
    			recevoirMessDGRAM(sock, (char *) buff);
    			deSerial(quoi, buff);
    		}
    	}
}
