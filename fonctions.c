#include "header.h"

//Définition des variables
SDL_Rect posMario;
SDL_Rect posLuigi;
SDL_Rect posMarioFireball;
SDL_Rect posLuigiFireball;
int idPerso;
int orientation_mario;
int orientation_luigi;
int tirMario;
int tirLuigi;


// Fonction de rafraichissement de l'écran (pour éviter de dupliquer ce code dans le main)
int frame(SDL_Surface *screen, SDL_Surface *fond, SDL_Surface *sol, SDL_Surface *plateforme, SDL_Surface *mario, SDL_Surface *luigi, SDL_Rect posFond, SDL_Rect posSol, SDL_Rect posPlateforme, SDL_Rect posMario, SDL_Rect posLuigi, SDL_Surface *marioFireball, SDL_Surface *luigiFireball, SDL_Rect posMarioFireball, SDL_Rect posLuigiFireball ) {

	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	SDL_BlitSurface(fond, NULL, screen, &posFond);
	SDL_BlitSurface(sol, NULL, screen, &posSol);
	SDL_BlitSurface(plateforme, NULL, screen, &posPlateforme);
	SDL_BlitSurface(luigi, NULL, screen, &posLuigi);
	SDL_BlitSurface(mario, NULL, screen, &posMario);
	SDL_BlitSurface(marioFireball, NULL, screen, &posMarioFireball);
	SDL_BlitSurface(luigiFireball, NULL, screen, &posLuigiFireball);

    SDL_Flip(screen); /* on rafraîchit */

    return 1;
}

// Initialise la connexion au serveur
int initClient(char ipAddress[], int port, int *sock)
 {
    struct sockaddr_in server;
    
    //Create socket
    *sock = socket(AF_INET , SOCK_STREAM , 0);
    if (*sock == -1)
    {
        printf("Socket non creee \n");
    }else{
        printf("Socket creee \n");
    }
    //parametrage de la connexion
    //On definit l IP, l AF et le port de connexion 
    server.sin_addr.s_addr = inet_addr(ipAddress);
    server.sin_family = AF_INET;
    server.sin_port = htons( port );//port de connexion


    printf("ipAddress : %s , port : %d\n", ipAddress, port);

    // Connexion au serveur
    if (connect(*sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        printf("Connexion echouee \n");
        printf("Error when connecting! %s\n",strerror(errno));
        return 0;
    }
    printf("Connexion reussie \n");
    return 1;
}

// Envoie un message par le scoket
int sendMessage(int sock, char message[])
{
    if(sock != 0)
    {                    
        //Envoi d un message au serveur
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            printf("Message non envoye \n");
            return 0;
        }
        return 1;
    }
    printf("Il faut initialiser le client\n"); 
    return 0;
}


// Fonction éxecutée par le thread d'éccoute
extern void *recevoir( void *arg)
{
    int read_size = 0, i, sock_tempo = (int)arg, posXInit;
    char client_message[200];
    memset(client_message, 0, sizeof(client_message)); 

    printf("Processus créé pour écouter les requetes clients \n");
    
    //Reception du message d un client
    while( (read_size = recv(sock_tempo , client_message , 200 , 0)) > 0)
    {  
         // Si confirmation de la deconnexion, on ferme le socket et quitte le thread d'écoute
         if(strstr(client_message, "deco_ok") != NULL)
         {
            close(sock_tempo);
            pthread_exit(EXIT_SUCCESS);
         }
         // Si c'est une réponse à la requete d'id du joueur
         else if(strstr(client_message, "ID_JOUEUR") != NULL)
         {
            sscanf(client_message, "ID_JOUEUR=%d", &idPerso);
            printf("Id joueur : %d\n", idPerso);
         }
         else
         {
            // De quel joueur doit-on récupérer la position ?
            if(strstr(client_message, "ID0") != NULL)
            {   
                posXInit = posMario.x;
                if(strstr(client_message, "TIR") != NULL)
                {
                    sscanf(client_message,"ID0:%d:%d:TIR:%d:%d", &(posMario.x),&(posMario.y),&(posMarioFireball.x), &(posMarioFireball.y));
                    tirMario=1;
                }
                else
                {
                    sscanf(client_message, "ID0:%d:%d", &(posMario.x), &(posMario.y));
                    tirMario=0;
                } 
                if(posMario.x < posXInit)
                {
                    orientation_mario = 1;
                }
                else
                {
                    orientation_mario = 0;
                }
            }
            else
            {
                posXInit = posLuigi.x;
                if(strstr(client_message, "TIR") != NULL)
                {
                    sscanf(client_message,"ID1:%d:%d:TIR:%d:%d", &(posLuigi.x),&(posLuigi.y),&(posLuigiFireball.x), &(posLuigiFireball.y));
                    tirLuigi=1;
                }
                else
                {
                    sscanf(client_message, "ID1:%d:%d", &(posLuigi.x), &(posLuigi.y));
                    tirLuigi=0;
                } 
                if(posLuigi.x < posXInit)
                {
                    orientation_luigi = 1;
                }
                else
                {
                    orientation_luigi = 0;
                }
            }
         }
         memset(client_message, 0, sizeof(client_message));              
    }
    if(read_size < 0){
        printf("Problème lors de la lecture du message : %s \n", strerror(errno));
        pthread_exit(0);
    }   
    pthread_exit(EXIT_SUCCESS);
}

// Retransforme une matrice de position sous forme de matrice
SDL_Rect unserialize(char *str)
{
    SDL_Rect pos;
    sscanf(str,"%d:%d:",&(pos.x),&(pos.y));
    return pos;
}


