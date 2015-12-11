/***********************************************
**              FICHIER OUTPUT.C              **
************************************************
**     Eloundou Cédric - Fauvet Guillaume     **
**                                            **
**              PROJET PROXY SERVER           **
************************************************/

#include "output.h"

//fonction ecrire message d'erreur
void ecrireErreur(const char * message, const int code){
	perror(" [X] %s" message);
	exit(code);
}

//Fonction ecrire message
void ecrireInfo(const char * message){
	printf(" [i] %s\n", message);
}