#include "server.h"

/*********** Fonction principale de l'application SERVEUR ***********
/*
/* Principe : On définit un tableau de socket pour stocker les connexions, on envoie ensuite un tableau
/* des coordonnées des surfaces aux clients sous forme d'un tableau d'int.
/* Puis on crée un thread d'écoute qui permet de réceptionner les messages avec les clients.
/* Le serveur envoie ensuite un tableau d'entiers de la surface concernée à chacun des clients.
/*
/*************************************************************/ 


int socket_desc, chuteMario, chuteLuigi;
int client_sock[NBMAXCLIENTS];
SDL_Rect posMario;
SDL_Rect posLuigi;
SDL_Rect posMarioFireball;
SDL_Rect posLuigiFireball;
int nbClient, sautMario, sautLuigi, chuteMario, chuteLuigi, tirMario, tirLuigi, chuteLuigi, chuteMario;
unsigned int tempsDebutSautMario, tempsDebutSautLuigi, tempsDebutTirMario, tempsDebutTirLuigi;
int orientation_marioFireball, orientation_luigiFireball, orientation_mario, orientation_luigi, depMario, depLuigi;

 //Envoi un message à l'adresse contenue dans le socket
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
        // printf("Message envoye\n");
        return 1;
    }
    printf("Il faut initialiser le client\n"); 
    return 0;
}



// Thread qui gère toutes les actions des personnages et les envoie aux clients
void *timeAction()
{
    unsigned int tempsActuel=0;
    char client_message0[200];
    char client_message1[200];
    char pos0[26];
    char pos1[26];

    while(1)
    {       
        memset(client_message0, 0, sizeof(client_message0));
        memset(client_message1, 0, sizeof(client_message1));
        memset(pos0, 0, sizeof(pos0));
        memset(pos1, 0, sizeof(pos1));
        strcpy(client_message0,"ID0:");
        strcpy(client_message1,"ID1:");
        tempsActuel = SDL_GetTicks();


        ///////////////////////////////////////////////////////////////////////////
        //////////////////////////      DEPD MARIO       //////////////////////////
        ///////////////////////////////////////////////////////////////////////////

        if(depMario == 1)
        {
            if (posMario.x >= LARGEUR-60) {
                 (posMario.x) = 0;
            }
            if ((posMario.x >= XPLATEFORME+230) && (posMario.y <= YPLATEFORME+5)) {
                chuteMario = 1;
            }
            (posMario.x)+=10;
            depMario = 0;
        }

        ///////////////////////////////////////////////////////////////////////////
        //////////////////////////      DEPG MARIO       //////////////////////////
        ///////////////////////////////////////////////////////////////////////////

        if(depMario == -1)
        {
            if (posMario.x <= 10) {
                 (posMario.x) = LARGEUR-60;
            }
            if ((posMario.x >= XPLATEFORME-70) && (posMario.y <= YPLATEFORME+5)) {
                chuteMario = 1;
            }
            (posMario.x)-=10;
            depMario = 0;
        }

        ///////////////////////////////////////////////////////////////////////////
        //////////////////////////      DEPD LUIGI       //////////////////////////
        ///////////////////////////////////////////////////////////////////////////

        if(depLuigi == 1)
        {
            if (posLuigi.x >= LARGEUR-60) {
                 (posLuigi.x) = 0;
            }
            if ((posLuigi.x >= XPLATEFORME+230) && (posLuigi.y <= YPLATEFORME+5)) {
                chuteLuigi = 1;
            }
            (posLuigi.x)+=10;
            depLuigi = 0;
        }

        ///////////////////////////////////////////////////////////////////////////
        //////////////////////////      DEPG LUIGI       //////////////////////////
        ///////////////////////////////////////////////////////////////////////////

        
        if(depLuigi == -1)
        {
            if (posLuigi.x <= 10) {
                 (posLuigi.x) = LARGEUR-60;
            }
            if ((posLuigi.x >= XPLATEFORME-70) && (posLuigi.y <= YPLATEFORME+5)) {
                chuteLuigi = 1;
            }
            (posLuigi.x)-=10;
            depLuigi = 0;
        }

        ///////////////////////////////////////////////////////////////////////////
        //////////////////////////      SAUT MARIO       //////////////////////////
        ///////////////////////////////////////////////////////////////////////////

        if ( tempsActuel - tempsDebutSautMario < 300) {
            // MONTEE
            if ((sautMario == 1) && (chuteMario == 0) ) {

                posMario.y-=15;
                 if (orientation_mario == 0)
                    posMario.x+=5;
                 else
                    posMario.x-=5;
            }
        }
        else if ( (sautMario == 1) && ( tempsActuel - tempsDebutSautMario > 300)){
            //DESCENTE

            if ((posMario.x >= XPLATEFORME-70 && posMario.x <= XPLATEFORME+240)) {
                //INTERIEUR
                if (posMario.y < (YPLATEFORME-100)) {
                    posMario.y+=15;
                     if (orientation_mario == 0)
                        posMario.x+=5;
                     else
                        posMario.x-=5;
                }
                else if ((posMario.y < (SOL-100)) && (posMario.y > (YPLATEFORME+5))) {
                    posMario.y+=15;
                    if (orientation_mario == 0)
                        posMario.x+=5;
                    else
                        posMario.x-=5;
                }
                else {
                    sautMario = 0;
                }
            }
            else {
                    //EXTERIEUR
                    if (posMario.y < (SOL-100)) {
                        posMario.y+=15;
                        if (orientation_mario == 0)
                            posMario.x+=5;
                        else
                            posMario.x-=5;
                    }
                    else {
                        sautMario = 0;
                    }
            }
        }

        ///////////////////////////////////////////////////////////////////////////
        //////////////////////////      SAUT LUIGI       //////////////////////////
        ///////////////////////////////////////////////////////////////////////////

        if ( tempsActuel - tempsDebutSautLuigi < 300) {
            // MONTEE
            if ((sautLuigi == 1) && (chuteLuigi == 0) ) {

                posLuigi.y-=15;
                 if (orientation_luigi == 0)
                    posLuigi.x+=5;
                 else
                    posLuigi.x-=5;
            }
        }
        else if ( (sautLuigi == 1) && ( tempsActuel - tempsDebutSautLuigi > 300)){
            //DESCENTE

                if ((posLuigi.x >= XPLATEFORME-70 && posLuigi.x <= XPLATEFORME+240)) {
                    //INTERIEUR
                    if (posLuigi.y < (YPLATEFORME-100)) {
                        posLuigi.y+=15;
                         if (orientation_luigi == 0)
                            posLuigi.x+=5;
                         else
                            posLuigi.x-=5;
                    }
                    else if ((posLuigi.y < (SOL-100)) && (posLuigi.y > (YPLATEFORME+5))) {
                        posLuigi.y+=15;
                        if (orientation_luigi == 0)
                            posLuigi.x+=5;
                        else
                            posLuigi.x-=5;
                    }
                    else {
                        sautLuigi = 0;
                    }
            }
            else {
                    //EXTERIEUR
                    if (posLuigi.y < (SOL-100)) {
                        posLuigi.y+=15;
                        if (orientation_luigi == 0)
                            posLuigi.x+=5;
                        else
                            posLuigi.x-=5;
                    }
                    else {
                        sautLuigi = 0;
                    }
            }
        }

        ///////////////////////////////////////////////////////////////////////////
        //////////////////////////      CHUTE MARIO       /////////////////////////
        ///////////////////////////////////////////////////////////////////////////

        if ((chuteMario == 1) && (sautMario == 0)) {
            if (posMario.y < (SOL-100)) {
                posMario.y+=15;
                if (orientation_mario == 0)
                    posMario.x+=5;
                else
                    posMario.x-=5;
            }
            else {
                chuteMario = 0;
            }
        }

        ///////////////////////////////////////////////////////////////////////////
        //////////////////////////      CHUTE LUIGI       /////////////////////////
        ///////////////////////////////////////////////////////////////////////////

        if ((chuteLuigi == 1) && (sautLuigi == 0)) {
            if (posLuigi.y < (SOL-100)) {
                posLuigi.y+=15;
                if (orientation_luigi == 0)
                    posLuigi.x+=5;
                else
                    posLuigi.x-=5;
            }
            else {
                chuteLuigi = 0;
            }
        }
        serialize(posMario, pos0);
        serialize(posLuigi, pos1);
        strcat(client_message0, pos0);
        strcat(client_message1, pos1);
        memset(pos0, 0, sizeof(pos0));
        memset(pos1, 0, sizeof(pos1));

        ///////////////////////////////////////////////////////////////////////////
        ////////////////////////////      TIR MARIO       /////////////////////////
        ///////////////////////////////////////////////////////////////////////////

        if ( tirMario &&  (tempsActuel - tempsDebutTirMario < (unsigned int) 2000)) {
            // printf("temps actuel : %u  tempsDebutTirMario : %u  dif : %d\n", tempsActuel, tempsDebutTirMario, tempsActuel-tempsDebutTirMario);
            if (orientation_marioFireball == 0){
                posMarioFireball.x+=10;
            }
            else {
                posMarioFireball.x-=10;
            }
            serialize(posMarioFireball, pos0);
            sprintf(client_message0, "%s:TIR:%s", client_message0, pos0);
        }
        else {
            // printf("tir fini\n");
            tirMario = 0;
            posMarioFireball.y= HAUTEUR + 40;
            posMarioFireball.x= LARGEUR + 50;
        }

        ///////////////////////////////////////////////////////////////////////////
        ////////////////////////////      TIR LUIGI       /////////////////////////
        ///////////////////////////////////////////////////////////////////////////

        if ( tempsActuel - tempsDebutTirLuigi < 2000) {
            if (tirLuigi == 1) {

                if (orientation_luigiFireball == 0){
                    posLuigiFireball.x+=10;
                }
                else {
                    posLuigiFireball.x-=10;
                }
                serialize(posLuigiFireball, pos1);
                sprintf(client_message1, "%s:TIR:%s", client_message1, pos1);
            }
        }
        else {
            tirLuigi = 0;
            posLuigiFireball.y= HAUTEUR + 40;
            posLuigiFireball.x= LARGEUR + 50;
        }

        

        for (int i = 0; i < NBMAXCLIENTS; ++i)
        {
            if(client_sock[i] != -1)
            {
                usleep(5000);
                sendMessage(client_sock[i], client_message0);
                usleep(5000);
                sendMessage(client_sock[i], client_message1);
            }
        }



    }
}

void *recevoir( void *arg)
{
    int read_size = 0, i, sock_tempo = (int)arg;
    char client_message[50];
    //vide la variable client_message pour éviter des problèmes de texte
    memset(client_message, 0, sizeof(client_message)); 

    printf("Processus créé pour écouter les requetes clients \n");
    
    //Reception du message d un client
    while( (read_size = recv(sock_tempo , client_message , 2000 , 0)) > 0)
    {    
         // Réception d'une requete de deconnexion du client
        if(strstr(client_message, "deco") != NULL)
        {
            // On parcourt le tableau de socket pour trouver velui correspondant
            for(i = 0; i<NBMAXCLIENTS; i++)
            {

                if(client_sock[i] == sock_tempo)
                {
                    memset(client_message, 0, sizeof(client_message));
                    // On confirme la deconnexion
                    strcpy(client_message,"deco_ok");
                    send(client_sock[i] , client_message , strlen(client_message),0);
                    // On ferme le socket et on l'efface du tableau
                    close(client_sock[i]);
                    client_sock[i] = -1;
                    nbClient--;
                    // On quitte le thread d'écoute de ce socket
                    pthread_exit(EXIT_SUCCESS);
                    break;                           
                }
                if(i == NBMAXCLIENTS -1)
                {
                    printf("Erreur socket non trouvé\n");
                }
            }
        }
        // Requete pour connaitre quel personnage sera utilisé
        else if(strstr(client_message, "ID_JOUEUR") != NULL)
        {
            for(i = 0; i<NBMAXCLIENTS; i++)
            {
                if(client_sock[i] == sock_tempo)
                {
                    // On renvoie l'id du joueur
                    memset(client_message, 0, sizeof(client_message));
                    sprintf(client_message, "ID_JOUEUR=%d",i);
                    send(client_sock[i] , client_message , strlen(client_message),0);
                    break;                           
                }
                if(i == NBMAXCLIENTS -1)
                {
                    printf("Erreur socket non trouvé\n");
                }
            }
        }
        else
        {    
            //On reçoit les données sous la forme RLJF:IDX où X est l'id du joueur
            //RLJF sont des paramètres qui représentent Right Left Jump et Fire

            // Si on appuie sur droite mais que gauche n'est pas enfoncé non plus (sinon on cela s'annule)
            if(strstr(client_message, "R") != NULL && strstr(client_message, "L") == NULL)
            {                
                // Si id=0 on bouge mario sinon luigi
                if(strstr(client_message, "ID0") != NULL)
                {
                    depMario = 1;
                    orientation_mario = 0;
                }
                else
                {
                    depLuigi = 1;
                    orientation_luigi = 0;
                }
            }
            // Idem pour déplacement gauche
            if(strstr(client_message, "L") != NULL && strstr(client_message, "R") == NULL)
            {                
                if(strstr(client_message, "ID0") != NULL)
                {
                    depMario = -1;
                    orientation_mario = 1;   
                }
                else
                {   
                    depLuigi = -1;
                    orientation_luigi = 1;
                }
            }   
            // Saut non géré pour l'instant
            if(strstr(client_message, "J") != NULL)
            {
                if(strstr(client_message, "ID0") != NULL)
                {
                    if (sautMario == 0){
                        sautMario = 1;
                        tempsDebutSautMario = SDL_GetTicks();
                    }
                }
                else
                {
                    if (sautLuigi == 0){
                        sautLuigi = 1;
                        tempsDebutSautLuigi = SDL_GetTicks();
                    }
                }
            }
            // Tir non géré pour l'instant
            if(strstr(client_message, "F") != NULL)
            {
                if(strstr(client_message, "ID0") != NULL)
                {
                    if (tirMario == 0){
                        printf("Tir\n");
                        tirMario = 1;
                        tempsDebutTirMario = SDL_GetTicks();
                    }
                }
                else
                {
                    if (tirLuigi == 0){
                        tirLuigi = 1;
                        tempsDebutTirLuigi = SDL_GetTicks();
                    }
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

// Mets la matrice de position sous forme de texte posx:posy
void serialize(SDL_Rect pos, char *str)
{
    /*Sint16 : 16 bits signed -32768->32768
    uint8 : 8bits unsigned 0 -> 255
    */
    sprintf(str,"%d:%d",pos.x,pos.y);
    return str;
}

// Retransforme une matrice de position sous forme de matrice
SDL_Rect unserialize(const char *str)
{
    SDL_Rect pos;
    sscanf(str,"%d:%d:",&(pos.x),&(pos.y));
    return pos;
}

int main(int argc , char *argv[])
{ 

    //atexit(terminus);

    
     int c , i, status, ret, sock_tempo;
     struct sockaddr_in server , client;
     pthread_t th, timeTh;
     
     char ch[5];
     char *seri;

     nbClient=0;

     // Initialisation des matrices position des 2 joueurs
    posMario.x = 0;
    posMario.y = SOL-100;

    posLuigi.x = LARGEUR-200;
    posLuigi.y = SOL-100;

    posMarioFireball.x = 0;
    posMarioFireball.y=0;

    posLuigiFireball.x =  0;
    posLuigiFireball.y = 0;

    orientation_mario= orientation_luigi= orientation_marioFireball= orientation_luigiFireball= nbClient= sautMario= sautLuigi= chuteMario= chuteLuigi= tirMario= tirLuigi= tempsDebutSautMario= tempsDebutSautLuigi= tempsDebutTirMario= tempsDebutTirLuigi =0;

    // Initialisation  du tableau de socket pour stocker les connexions
     for (i = 0; i < NBMAXCLIENTS; i++)
     {
        client_sock[i] = -1;
     }
      
     //Create socket
     socket_desc = socket(AF_INET , SOCK_STREAM , 0);
     if (socket_desc == -1)
     {
         printf("Socket non creee \n");
     }else{
         printf("Socket creee \n");
     }     
     //Preparation de sockaddr_in structure
     server.sin_family = AF_INET;
     server.sin_addr.s_addr = INADDR_ANY;
     server.sin_port = htons( 8888 );// port d ecoute
     //Bind
     if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
     {
        //Afficher un message d erreur
          printf("bind echoué. Erreur \n");
          printf("Error when connecting! %s\n",strerror(errno));
         return 1;
     }else{
         printf("bind reussi \n");
     }
     
     //Ecoute sur le port
     listen(socket_desc , NBMAXCLIENTS);
	printf("Attente d'arrivee des connexions... \n");

    status = pthread_create(&timeTh,NULL, timeAction, NULL);
    if(status == -1)
    {
        perror("Erreur création thread actions\n");
    }
    
    	while(1){
     
	     //Accepte une connection entrante
		c = sizeof(struct sockaddr_in);
	    
	     //Acception de la connexion arrivant d'un client
	     sock_tempo = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	     	     
		if (sock_tempo < 0)
		{
		 	printf("acceptation echouee : %s\n", strerror(errno));
			return 0;
		}
        else
        {		
        	// On ajoute la nouvelle connexion dans le tableau
            if(nbClient < NBMAXCLIENTS)
            { 
                printf("acceptation reussie \n");               
                client_sock[nbClient] = sock_tempo;
                nbClient++;
            }
		}		             

        // On créé un thread d'écoute pour cette connexion
	     status = pthread_create(&th, NULL, recevoir, (void *)sock_tempo);
         if(status==-1)
         {
            perror("pthread create");
         }     
    }    
    return 1;
     
}
