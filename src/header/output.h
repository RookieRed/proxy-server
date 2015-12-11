/***********************************************
**              FICHIER OUTPUT.H              **
************************************************
**     Eloundou Cédric - Fauvet Guillaume     **
**                                            **
**              PROJET PROXY SERVER           **
************************************************/

#include "types.h"

/**
* Ecrit à l'écran un message d'erreur avec son code
* @param message : le message à afficher
*/
void ecrireErreur(const char * message, const int code);

/**
* Ecrit à l'écran un message d'information
* @param message : le message à afficher
*/
void ecrireInfo(const char * message);