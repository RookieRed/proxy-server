/***********************************************
**                 FICHER PROXY.H             **
************************************************
**     Eloundou Cédric - Fauvet Guillaume     **
**                                            **
**              PROJET PROXY SERVER           **
************************************************/


void creerSocketServeur(){

	struct addrinfo hints, *res;
	Socket socFD;
	int err;


	//Création de la socket
	socFD = socket(AF_INET, SOCK_STREAM, 0);
	if (socFD == 0) {
		ecrireErreur("Erreur socket", 1);
	}

	//Récupération des address info
	memset()


}