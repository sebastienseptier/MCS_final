#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <string.h>     //strlen
#include <sys/socket.h> //socket
#include <arpa/inet.h>  //inet_addr
#include <errno.h>  //erreur reseau
#include <unistd.h> //fork
#include <signal.h>  //kill
#include <sys/types.h> //get_pid
#include <pthread.h>

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

//Définition d'un type chaine de caracteres
typedef char TCHAINE[MAX];

extern SDL_Rect posMario;
extern SDL_Rect posLuigi;
extern SDL_Rect posMarioFireball;
extern SDL_Rect posLuigiFireball;
extern int idPerso;
extern int orientation_mario;
extern int orientation_luigi;
extern int tirMario;
extern int tirLuigi;

//Prototypes des fonctions

int initClient(char ipAddress[], int port, int *sock);
int sendMessage(int sock, char message[]);
char *serialize(SDL_Rect pos);
SDL_Rect unserialize(char str[]);
void right(SDL_Rect *perso, int *orientation, int *chute, int sock);
void left(SDL_Rect *perso, int *orientation,  int *chute, int sock);
int frame(SDL_Surface *screen, SDL_Surface *fond, SDL_Surface *sol, SDL_Surface *plateforme, SDL_Surface *mario, SDL_Surface *luigi, SDL_Rect posFond, SDL_Rect posSol, SDL_Rect posPlateforme, SDL_Rect posMario, SDL_Rect posLuigi, SDL_Surface *marioFireball, SDL_Surface *luigiFireball, SDL_Rect posMarioFireball, SDL_Rect posLuigiFireball );
extern void *recevoir(void *arg);
