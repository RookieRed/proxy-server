// Bibliothéques 
// ==============================================

#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/socket.h>
#include  <netdb.h>
#include  <string.h>


// Constantes
// ==============================================

#define SERVADDR     "127.0.0.1"        // Définition de l'adresse IP d'écoute
#define SERVPORT     "4444"                // Définition du port d'écoute, si 0 port choisi dynamiquement
#define LISTENLEN     2                 // Taille du tampon de demandes de connexions, MAX 2
#define MAXBUFFERLEN  1024
#define MAXHOSTLEN    64
#define MAXPORTLEN    6


// PROCEDURES
// ==============================================

void decoupeLogin( char *entreeUtilisateur, char *login, char *server );
void waitingForUser(int *socket,char *buffer,int *n);
void waitingForServer(int *socket,char *buffer,int *n);


// MAIN
// ==============================================

int main( int argc, char *argv[] ) {

    // Variables [ SERVEUR ] >> [ PROXY ] - [ PROXY ] >> [ SERVEUR ]
    // ==============================================

    int sockServeur;                                                                // Descripteur pour le socket
    int fncRet;                                                                     // Stock le retour des fonctions
    struct addrinfo *resBIS;                                                        // Résultat de la focntion getaddrinfo
    struct addrinfo hintsBIS = {0, AF_INET, SOCK_STREAM, 0, 0, NULL, NULL, NULL};   // Filtra pour le getaddrinfo
    char bufferFTP[MAXBUFFERLEN];

    // Variables [ CLIENT ] >> [ PROXY ] - [ PROXY ] >> [ CLIENT ]
    // ==============================================

    char            serverAddr[MAXHOSTLEN];     // Adresse du serveur
    char            serverPort[MAXPORTLEN];     // Port du server
    char            buffer[MAXBUFFERLEN];       // Tampon pour  écrire sur le socket
    char            bufferRcv[MAXBUFFERLEN];    // Tampon pour  écrire sur le socket
    int             descSockRDV;                // Descripteur de socket de rendez-vous
    int             ecode;                      // Code retour des fonctions
    int             descSockCOM;                // Descripteur de socket de communication
    int             n = 0;
    int             lenRCV = 0;


    struct addrinfo         hints;   // Filtre pour la fonction get_addr_info
    struct addrinfo         *res;    // Resultat get_addr_info
    struct sockaddr_storage myinfo;  // Informations sur la connexion de RDV
    struct sockaddr_storage from;    // Informations sur le client connecté


    char entreeUtilisateur[40];  // login@server
    char login[20];             // Isoler le login
    char serveur[20];             // Isoler le serveur


    socklen_t               len = sizeof( struct sockaddr_storage );  // Variable utilisée pour stocker les longueurs des structures de socket

    pid_t pid;				

    // Initialisation de la socket de RDV IPv4/TCP

    descSockRDV = socket( AF_INET, SOCK_STREAM, 0 );
   
   // Mise à zéro de la variable hints,entreeUtilisateur,loggin..

    memset( &hints, 0, sizeof( hints ) );
    memset( entreeUtilisateur, 0, sizeof( entreeUtilisateur ) );
    memset( login, 0, sizeof( login ) );
    memset( serveur, 0, sizeof( serveur ) );
    memset( buffer, 0, sizeof( MAXBUFFERLEN ) );
    memset( bufferRcv, 0, sizeof( MAXBUFFERLEN ) );
    memset( bufferFTP, 0, sizeof( MAXBUFFERLEN ) );

    // Initailisation de la variable hints 

    hints.ai_flags = AI_PASSIVE;      // mode serveur, nous allons utiliser la fonction bind
    hints.ai_socktype = SOCK_STREAM;  // TCP
    hints.ai_family = AF_INET;        // IPv4

    // Récupération des informations du serveur > Machine locale

    ecode = getaddrinfo( SERVADDR, SERVPORT, &hints, &res );
  
    // Publication de la socket

    ecode = bind( descSockRDV, res->ai_addr, res->ai_addrlen );

    ecode = listen( descSockRDV, LISTENLEN );
   
    // Attente connexion du client
    // Lorsque demande de connexion, creation d'un processus fils et d'un socket de communication avec le client, MAX 2 attentes

    while( 1 == 1 ) {

        descSockCOM = accept( descSockRDV, ( struct sockaddr * ) &from, &len );

        if( ( pid= fork() ) == 0) {

            close( descSockRDV );  // On ferme le socketRDV on s'occupe seulement de gérer le socket actuel

            strcpy( buffer, "220 Bienvenue sur le proxyFTP de aurehacks\r\n" );  // Echange de données avec le client connecté

            send( descSockCOM, buffer, strlen( buffer ), 0 ); // Ecriture sur le socket

            while(1 == 1) { // Gestion des commandes

                waitingForUser(&descSockCOM,bufferRcv,&n);

                // Gestion de l'authentification
                // ==============================================

                if( strstr( bufferRcv,"USER") ) {

                    memset( buffer, 0, sizeof( MAXBUFFERLEN ) );

                    decoupeLogin(bufferRcv,login,serveur); // On isole le loggin ainsi que l'IP/URL du serveur

                    strcat(buffer,login); // On formate le login

                    sockServeur = socket(AF_INET, SOCK_STREAM, 0);

                    getaddrinfo(serveur,"21",&hints,&res);

                    connect(sockServeur, res->ai_addr, res->ai_addrlen);



                     // [1] Message de Bienvenue 
                    // ==============================================

                    waitingForServer(&sockServeur,bufferFTP,&n);

                    // ==============================================



                     // [2] Envoi de "USER login"
                    // ==============================================

                    send( sockServeur, buffer, strlen( buffer ), 0 );
                    send( sockServeur, "\r\n", 2, 0 );

                    waitingForServer(&sockServeur,bufferFTP,&n);
 
                    // ==============================================


                    // [3] Réception du "331 password required" et transmission au client
                    // ==============================================

                    strcpy( bufferFTP, "331 required password\r\n" ); // Cas special SINON ON EN DIRECT send( descSockCOM, strcat(bufferFTP,"\r\n"), strlen( bufferFTP ), 0 );

                    send( descSockCOM, bufferFTP, strlen( bufferFTP ), 0 );

                    waitingForUser(&descSockCOM,bufferRcv,&n);

                    // ==============================================



                    // [4] Réception du "PASS mdp" (mot de passe du client) et transmission au serveur
                    // ==============================================

                    send( sockServeur, bufferRcv, strlen( bufferRcv ), 0 );
                    send( sockServeur, "\r\n", 2, 0 );

                    waitingForServer(&sockServeur,bufferFTP,&n);

                    // ==============================================



                    // [5] Réception du "230 logged in" et transmission au client
                    // ==============================================

                    strcpy( bufferFTP, "230 Logged IN !\r\n" ); // Cas special SINON ON EN DIRECT send( descSockCOM, strcat(bufferFTP,"\r\n"), strlen( bufferFTP ), 0 );

                    send( descSockCOM, bufferFTP, strlen( bufferFTP ), 0 );

                    waitingForUser(&descSockCOM,bufferRcv,&n);



                    // [6] Transfert de la commande "SYST"
                    // ==============================================

 					send( sockServeur, bufferRcv, strlen( bufferRcv ), 0 );

                    send( sockServeur, "\r\n", 2, 0 );

                    waitingForServer(&sockServeur,bufferFTP,&n);

                    // ==============================================



                    // [7] Réception du résultat de SYST, ajout de '\r\n' pour format FTP
                    // ==============================================

                    strcat(bufferFTP, "\r\n");

					send( descSockCOM, bufferFTP, strlen( bufferFTP ), 0 );

                    waitingForUser(&descSockCOM,bufferRcv,&n);
					

                    // ==============================================



                    // [8] Transfert de la commande ls qui déclenche un PORT
                    // ==============================================

                    send( sockServeur, bufferRcv, strlen( bufferRcv ), 0 );

                    send( sockServeur, "\r\n", 2, 0 );


                        // Mode actif pour [ CLIENT ] >> [ PROXY ]

                        // socket pour se connecter au client


                        // Mode passif pour [ PROXY ] >> [ CLIENT ]

                        // socket pour réceptionner les données du serveur



                    // ==============================================



                    // On clean les buffers

                    memset( buffer, 0, sizeof( MAXBUFFERLEN ) );
                    memset( bufferRcv, 0, sizeof( MAXBUFFERLEN ) );
                    memset( bufferFTP, 0, sizeof( MAXBUFFERLEN ) );


                }


                 // Gestion du mode actif
                // ==============================================

                if( strncmp( bufferRcv,"PORT", 4 ) ) {
                    n = 0;

                    // On clean les buffers

                    memset( buffer, 0, sizeof( MAXBUFFERLEN ) );
                    memset( bufferRcv, 0, sizeof( MAXBUFFERLEN ) );
                    memset( bufferFTP, 0, sizeof( MAXBUFFERLEN ) );

                    
                }

                 // Gestion du mode passif
                // ==============================================

                if( strncmp( bufferRcv,"PASSV", 5 ) ) {

                    n = 0;

                    // On clean les buffers

                    memset( buffer, 0, sizeof( MAXBUFFERLEN ) );
                    memset( bufferRcv, 0, sizeof( MAXBUFFERLEN ) );
                    memset( bufferFTP, 0, sizeof( MAXBUFFERLEN ) );

                    
                }

                // Gestion de la fermeture de session
                // ==============================================


                if( strncmp( bufferRcv,"QUIT", 4 ) ) {

                    break; // On quitte la boucle lorsque que l'on reçoit la commande "QUIT"
                }

            }

            freeaddrinfo(res);

            close( descSockCOM );
            close( sockServeur );

            // Fin du fils

            exit(0);
        }
    }

    close( descSockRDV );

  return 0;
}

// decoupeLogin
// ==============================================

void decoupeLogin( char *entreeUtilisateur, char *login, char *server ) {

    int i,j = 0;

    for( i=0; i<strlen( entreeUtilisateur ); i++) {

        if( entreeUtilisateur[i] != '@' )
            login[i] = entreeUtilisateur[i];

        if( entreeUtilisateur[i] == '@' ) {

            i++;

            while( entreeUtilisateur[i] != '\0' ) {

                server[j] = entreeUtilisateur[i];
                i++;
                j++;
            }
        }
    }

}

void waitingForUser(int *socket,char *buffer,int *n) {

    *n = 0;

     while(1 == 1) { // Gestion du buffer

        recv( *socket, buffer+( *n ), 1, 0 );

        if( buffer[*n] == '\n' && buffer[( *n )-1] == '\r') {

            buffer[*n]       = '\0';
            buffer[( *n )-1] = '\0';
            break;
        }

        ( *n )++;

    }

}

void waitingForServer(int *socket,char *buffer,int *n) {

    *n = 0;

    while(1 == 1) { // Gestion du buffer

        recv( *socket, buffer+( *n ), 1, 0 );

        if( buffer[*n] == '\n' && buffer[( *n )-1] == '\r') {

            buffer[*n]       = '\0';
            buffer[( *n )-1] = '\0';
            break;
        }

        ( *n )++;

    }
}

