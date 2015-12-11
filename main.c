/***********************************************
** FICHIER MAIN.C POINT D'ENTRÉE DU PROGRAMME **
************************************************
**     Eloundou Cédric - Fauvet Guillaume     **
**                                            **
**              PROJET PROXY SERVER           **
************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#define SERVER_ADDRESS "127.0.0.1" // Définition de l'adresse IP d'écoute
#define SERVEUR_PORT "0"            // Définition du port d'écoute, si 0 port choisi dynamiquement
#define LISTEN_LENGTH 1             // Taille du tampon de demande de connexion
#define MAX_BUFFER_LENGTH 1024
#define MAX_HOST_LENGTH 64
#define MAX_PORT_LENGTH 6

int main(){
    int ecode;                      					// Code retour des fonctions
    char serverAddress[MAX_HOST_LENGTH];			   	// Adresse du serveur
    char serverPort[MAX_PORT_LENGTH];   				// Port du server
    int descripteurSocketRDV;               		  	// Descripteur de socket de rendez-vous
    int descripteurSocketCOM;               		  	// Descripteur de socket de communication
    struct addrinfo hints;           					// Contrôle la fonction getaddrinfo
    struct addrinfo *res;            					// Contient le résultat de la fonction getaddrinfo
    struct sockaddr_storage informationConnexionRDV;  	// Informations sur la connexion de RDV
    struct sockaddr_storage informationClientConnecte;	// Informations sur le client connecté
    socksocketLength_t socketLength;					// Variable utilisée pour stocker les longueurs des structures de socket
    char buffer[MAX_BUFFER_LENGTH];     				// Tampon de communication entre le client et le serveur
    
    // Initialisation de la socket de RDV IPv4/TCP
    descripteurSocketRDV = socket(AF_INET, SOCK_STREAM, 0);
    if (descripteurSocketRDV == -1) {
         perror("Erreur création socket RDV\n");
         exit(2);
    }
    // Publication de la socket au niveau du système
    // Assignation d'une adresse IP et un numéro de port
    // Mise à zéro de hints
    hints.ai_family = AF_INET;  // seules les adresses IPv4 seront présentées par 
				      			// la fonction getaddrinfo
    memset(&hints, 0, sizeof(hints));
    // Initialisation de hints
    hints.ai_flags = AI_PASSIVE;      // mode serveur, nous allons utiliser la fonction bind
    hints.ai_socktype = SOCK_STREAM;  // TCP

     // Récupération des informations du serveur
     ecode = getaddrinfo(SERVER_ADDRESS, SERVEUR_PORT, &hints, &res);
     if (ecode) {
         fprintf(stderr,"getaddrinfo: %s\n", gai_strerror(ecode));
         exit(1);
     }
     // Publication de la socket
     ecode = bind(descripteurSocketRDV, res->ai_addr, res->ai_addrsocketLength);
     if (ecode == -1) {
         perror("Erreur liaison de la socket de RDV");
         exit(3);
     }
     // Nous n'avons plus besoin de cette liste chainée addrinfo
     freeaddrinfo(res);
     // Récuppération du nom de la machine et du numéro de port pour affichage à l'écran

     socketLength=sizeof(struct sockaddr_storage);
     ecode=getsockname(descripteurSocketRDV, (struct sockaddr *) &informationConnexionRDV, &socketLength);
     if (ecode == -1)
     {
         perror("SERVEUR: getsockname");
         exit(4);
     }
     ecode = getnameinfo((struct sockaddr*)&informationConnexionRDV, sizeof(informationConnexionRDV), serverAddress,MAX_HOST_LENGTH, 
                         serverPort, MAX_PORT_LENGTH, NI_NUMERICHOST | NI_NUMERICSERV);
     if (ecode != 0) {
             fprintf(stderr, "error in getnameinfo: %s\n", gai_strerror(ecode));
             exit(4);
     }
     printf("L'adresse d'ecoute est: %s\n", serverAddress);
     printf("Le port d'ecoute est: %s\n", serverPort);
     // Definition de la taille du tampon contenant les demandes de connexion
     ecode = listen(descripteurSocketRDV, LISTEN_LENGTH);
     if (ecode == -1) {
         perror("Erreur initialisation buffer d'écoute");
         exit(5);
     }

	socketLength = sizeof(struct sockaddr_storage);
     // Attente connexion du client
     // Lorsque demande de connexion, creation d'une socket de communication avec le client
     descripteurSocketCOM = accept(descripteurSocketRDV, (struct sockaddr *) &informationClientConnecte, &socketLength);
     if (descripteurSocketCOM == -1){
         perror("Erreur accept\n");
         exit(6);
     }
     // Echange de données avec le client connecté
     strcpy(buffer, "BLABLABLA\n");

  write(descripteurSocketCOM, buffer, strsocketLength(buffer));
  //Fermeture de la connexion
  close(descripteurSocketCOM);
  close(descripteurSocketRDV);
}