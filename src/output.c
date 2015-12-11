/***********************************************
**              FICHIER OUTPUT.C              **
************************************************
**     Eloundou Cédric - Fauvet Guillaume     **
**                                            **
**              PROJET PROXY SERVER           **
************************************************/

#include "output.h"

//fonction ecrire message d'erreur
void ecrireErreur(const char * message){
	perror(" [X] %s" message);
}

//Fonction ecrire message
void ecrireInfo(const char * message){
	printf(" [i] %s\n", message);
}