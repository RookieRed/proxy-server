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
    
}