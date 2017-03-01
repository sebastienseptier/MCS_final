#include <stdio.h>
#include <stdlib.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <errno.h>  //erreur reseau
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <sys/types.h> //getpid
#include <pthread.h>
#include <SDL/SDL.h>

#define NBMAXCLIENTS 2 //nombre maximum de client pouvant se connecter sur le serveur

//Definition des positions et des dimensions
#define LARGEUR 800
#define HAUTEUR 600
#define SOL 480
#define XPLATEFORME 275
#define YPLATEFORME 330
#define MAXTAB 5
#define MAX 20

//Macro fonction
#define VIDE(ch) (strlen(ch)==0)


//Prototypes des fonctions : Envoi un message à l'adresse contenue dans le socket
int sendMessage(int sock, char message[]);
void *recevoir( void *arg);
void serialize(SDL_Rect pos, char* str);
SDL_Rect unserialize(const char str[]);
