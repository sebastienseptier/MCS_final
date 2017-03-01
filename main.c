#include "header.h"

/*********** Fonction principale de l'application CLIENT ***********
/*
/* Principe : On définit les surfaces + coordonées au début, puis on rentre dans une boucle infinie.
/* La variable "choix" décide de l'affichage, pour telle valeur de choix, il y a un switch-case qui détecte
/* les touches appuyées. Dans chacun de ces cas, on agit sur les variables (coordonnées, flag, ou choix),
/* puis on termine par rafraichir l'ensemble de la fenêtre grâce à la fonction frame().
/* L'affichage du jeu (choix = 3) a des fonctions en plus pour gérer les cas de saut, chute, et attaque.
/*
/*************************************************************/


int main(int argc, char* argv[])
{
    /******************************** DECLARATIONS ********************************/

    // Déclaration des surfaces

    SDL_Surface *sol = NULL, *plateforme = NULL;
    SDL_Surface *mario, *luigi, *marioFireball, *luigiFireball;

    SDL_Surface *screen = NULL, *fond = NULL,
    *titre = NULL, *soustitre = NULL, *btconnecter = NULL, *btinscrire = NULL,
    *btcreer = NULL, *btrejoindre = NULL, *btregarder = NULL, *btquitter = NULL, *imageConection = NULL,
    *textIp, *textMdp, *textPseudo, *textInfo, *textPseudoLabel, *textMdpLabel, *textIpLabel;

    // Déclaration d'un événement (valeur différente suivant la touche appuyée)
    SDL_Event event;

     char action[5];
     char str[50];

    // Déclaration des positions (certaines sont fixes, d'autres sont utilisée pour plsrs surfaces)
    SDL_Rect position;
    SDL_Rect posFond, posSol, posPlateforme;
    SDL_Rect posMarioFireball, posLuigiFireball;

    // Déclaration des polices & couleurs
    TTF_Font *policeTitre = NULL, *policeBt = NULL;
    SDL_Color couleurNoire = {0, 0, 0};
    SDL_Color couleurBlanche = {255, 255, 255};
    SDL_Color couleurBleue = {50, 100, 155};

    // Déclaration des variables (flag,..etc)
    int ouvert = 1, i=1, j=0, k=0,coordX=0, coordY=0,choix = 1, sens = 0;

    int up = 0, len = 0, mario_atterir = 1;
    int tempsActuel = 0, etatConnection = 0;
    orientation_mario = 0;
    orientation_luigi = 0;

    tirMario = 0;
    tirLuigi = 0;

    unsigned int index=0, fin=0;

    // Définitions des champs à entrer
    TCHAINE pseudo = "", mdp = "", ip = "";
    char lettre;

    // Socket
    int sock = 0;
    int isConnected = 0;

    pthread_t th;


    /******************************** CONFIGURATION DE L'AFFICHAGE ********************************/


    if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("Unable to init SDL : %s\n", SDL_GetError() );
    }
    TTF_Init();

    SDL_WM_SetIcon(SDL_LoadBMP("images/mario.bmp"), NULL);
    putenv("SDL_VIDEO_WINDOW_POS=center");


    screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);


    if ( screen == NULL)
    {
        fprintf(stderr, "Imposible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_WM_SetCaption("Atomic Killer Reloaded+", NULL);


    /******************************** CONFIGURATION ********************************/

        /* DES SURFACES */

    policeTitre = TTF_OpenFont("polices/secret.otf", 45);
    if(!policeTitre)
    {
      printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
      exit(3);
    }

    policeBt = TTF_OpenFont("polices/united.ttf", 35);
    if(!policeBt)
    {
      printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
      exit(3);
    }

    fond = IMG_Load("images/fond_menu.png");

    textPseudo = TTF_RenderText_Blended(policeBt, pseudo, couleurNoire);
    textMdp = TTF_RenderText_Blended(policeBt, mdp, couleurNoire);
    textIp = TTF_RenderText_Blended(policeBt, ip, couleurNoire);
    textInfo = TTF_RenderText_Blended(policeBt, "Veuillez entrer votre pseudo svp", couleurNoire);

    titre = TTF_RenderText_Blended(policeTitre, "Atomic Killer Reloaded+:", couleurNoire);
    soustitre = TTF_RenderText_Blended(policeTitre, "GOTY Premium Edition", couleurNoire);
    btconnecter = TTF_RenderText_Blended(policeBt, "Se connecter", couleurNoire);


    btcreer = TTF_RenderText_Blended(policeBt, "Creer", couleurBleue);
    btrejoindre = TTF_RenderText_Blended(policeBt, "Rejoindre", couleurBlanche);
    btregarder = TTF_RenderText_Blended(policeBt, "Regarder", couleurBlanche);
    btquitter = TTF_RenderText_Blended(policeBt, "Quitter", couleurBlanche);

    // Chargement affichage

    fond = IMG_Load("images/fond_jeu.png");
    posFond.x = 0;
    posFond.y = 0;
    SDL_BlitSurface(fond, NULL, screen, &posFond);

    sol = IMG_Load("images/sol.png");
    posSol.x = 0;
    posSol.y = SOL;
    SDL_BlitSurface(sol, NULL, screen, &posSol);

    plateforme = IMG_Load("images/plateforme.png");
    posPlateforme.x = XPLATEFORME;
    posPlateforme.y = YPLATEFORME;
    SDL_BlitSurface(plateforme, NULL, screen, &posPlateforme);

    mario = IMG_Load("images/mario_right_down.png");
    posMario.x = 0;
    posMario.y = SOL-100;
    SDL_BlitSurface(mario, NULL, screen, &posMario);

    marioFireball = IMG_Load("images/mario_right_fireball.png");
    posMarioFireball.x = 80;
    posMarioFireball.y = SOL-160;
    SDL_BlitSurface(marioFireball, NULL, screen, &posMarioFireball);

    luigi = IMG_Load("images/luigi_left_down.png");
    posLuigi.x = LARGEUR-200;
    posLuigi.y = SOL-100;
    SDL_BlitSurface(luigi, NULL, screen, &posLuigi);

    luigiFireball = IMG_Load("images/luigi_right_fireball.png");
    posLuigiFireball.x = 80;
    posLuigiFireball.y = SOL-160;
    SDL_BlitSurface(luigiFireball, NULL, screen, &posLuigiFireball);

    imageConection = IMG_Load("images/disconnected.png");
    position.x = 0;
    position.y = 0;
    SDL_BlitSurface(imageConection, NULL, screen, &position);



    /******************************** BOUCLE INFINIE ********************************/


    while (ouvert)
    {
        if (choix == 1) {

                /***** CHOIX 1 : AFFICHAGE MENU CONNEXION *****/

                SDL_WaitEvent(&event);


                switch(event.type)
                {
                    case SDL_QUIT:
                        ouvert = 0;
                    break;

                    case SDL_KEYDOWN :

                        lettre=event.key.keysym.sym;
                        int lettre_int = event.key.keysym.sym;

                        if(etatConnection == 2)
                        {
                          if ( (lettre >= SDLK_0 && lettre <= SDLK_9) || lettre == SDLK_PERIOD )
                          {
                            ip[index] = lettre;
                            index++;
                          }
                          else if(lettre_int == SDLK_PERIOD || lettre_int == SDLK_KP_PERIOD)
                          {
                            ip[index]='.'; 
                            index++;
                          }
                          else if((lettre_int >= SDLK_KP0 && lettre_int <= SDLK_KP9))
                          {
                              ip[index]= lettre + 48;
                              index++; //on se prépare à recevoir la prochaine lettre                            
                          }
                          SDL_Flip(screen);
                        } 
                        else
                        {
                          if ((lettre>=SDLK_a && lettre<=SDLK_z) || (lettre>=SDLK_0 && lettre<=SDLK_9) || lettre==SDLK_PERIOD)
                          {
                              //si entre a et z ou 0-9 ou .
                              if (etatConnection == 0)
                                  pseudo[index]=lettre; //on ajoute la lettre
                              if (etatConnection == 1)
                                  mdp[index]=lettre;
                              index++; //on se prépare à recevoir la prochaine lettre
                              SDL_Flip(screen);
                          }
                        }


                        switch(event.key.keysym.sym)
                        {
                            case SDLK_ESCAPE :
                                ouvert = 0;
                            break;

                            case SDLK_BACKSPACE :
                              if(index != 0)
                              {
                                index--;
                                if(etatConnection == 0)
                                pseudo[index] = ' ';
                                if(etatConnection == 1)
                                  mdp[index] = ' ';
                                if(etatConnection == 2)
                                  ip[index] = ' ';
                              }
                              break;

                            case SDLK_RETURN:
                                if ((etatConnection == 0) && !VIDE(pseudo)){
                                    etatConnection = 1;
                                    textInfo = TTF_RenderText_Blended(policeBt, "Veuillez entrer votre mot de passe svp", couleurNoire);
                                    pseudo[index]='\0'; // on met la fin de chaine
                                    index=0;
                                }
                                else if ((etatConnection == 1) && !VIDE(mdp)) {
                                    etatConnection = 2;
                                    textInfo = TTF_RenderText_Blended(policeBt, "Adresse ip du pc de l'adversaire ?", couleurNoire);
                                    mdp[index]='\0';
                                    index=0;
                                }
                                else if ((etatConnection == 2) && !VIDE(ip)) {
                                    etatConnection = 3;
                                    ip[index]='\0';
                                    index=0;
                                }
                                else
                                    choix = 2;
                                    int status;
                                   status = pthread_create(&th, NULL, recevoir, (void *)sock);
                                 if(status==-1)
                                 {
                                    perror("pthread create");
                                 }
                            break;
                        }//switch

                    break;
                }

                // Choix des couleurs des champs suivant l'état d'avancement
                if (etatConnection == 0 || etatConnection == 1 || etatConnection == 2) {
                    textPseudoLabel = TTF_RenderText_Blended(policeBt, "pseudo : ", couleurBlanche);
                    textPseudo = TTF_RenderText_Blended(policeBt, pseudo, couleurBlanche);
                    textMdpLabel = TTF_RenderText_Blended(policeBt, "mdp : ", couleurNoire);
                    textMdp = TTF_RenderText_Blended(policeBt, mdp, couleurNoire);
                    textIpLabel = TTF_RenderText_Blended(policeBt, "adresse ip : ", couleurNoire);
                    textIp = TTF_RenderText_Blended(policeBt, ip, couleurNoire);
                }
                else if (etatConnection == 3) {
                    textPseudo = TTF_RenderText_Blended(policeBt, pseudo, couleurBleue);
                    textMdp = TTF_RenderText_Blended(policeBt, mdp, couleurBleue);
                    textIp = TTF_RenderText_Blended(policeBt, ip, couleurBleue);
                    btconnecter = TTF_RenderText_Blended(policeBt, "Se connecter", couleurBlanche);
                    // On se connecte au serveur avec l'adresse ip rentrée
                    if(!isConnected)
                    {
                        isConnected = initClient(ip, 8888, &sock);
                        // On demande au serveur quel joueur on controlera
                        sendMessage(sock, "ID_JOUEUR");
                    }
                }

                // Rafraichissement
                position.x = 0;
                position.y = 0;
                SDL_BlitSurface(fond, NULL, screen, &position);

                position.x = 140;
                position.y = 10;
                SDL_BlitSurface(titre, NULL, screen, &position);

                position.x = 200;
                position.y = 60;
                SDL_BlitSurface(soustitre, NULL, screen, &position);

                position.x = 100;
                position.y = 200;
                SDL_BlitSurface(textPseudoLabel, NULL, screen, &position);

                position.x = 260;
                position.y = 200;
                SDL_BlitSurface(textPseudo, NULL, screen, &position);

                position.x = 150;
                position.y = 280;
                SDL_BlitSurface(textMdpLabel, NULL, screen, &position);

                position.x = 260;
                position.y = 280;
                SDL_BlitSurface(textMdp, NULL, screen, &position);

                position.x = 50;
                position.y = 360;
                SDL_BlitSurface(textIpLabel, NULL, screen, &position);

                position.x = 260;
                position.y = 360;
                SDL_BlitSurface(textIp, NULL, screen, &position);

                position.x = 100;
                position.y = HAUTEUR-35;
                SDL_BlitSurface(textInfo, NULL, screen, &position);

                position.x = 260;
                position.y = 440;
                SDL_BlitSurface(btconnecter, NULL, screen, &position);


                SDL_Flip(screen);
        }//if
        else if (choix == 2) {

                /***** CHOIX 2 : AFFICHAGE MENU DU JEU *****/


                SDL_WaitEvent(&event);


                switch(event.type)
                {
                    case SDL_QUIT:
                        ouvert = 0;
                    break;

                    case SDL_KEYDOWN :
                        switch(event.key.keysym.sym)
                        {
                            case SDLK_ESCAPE :
                                ouvert = 0;
                            break;

                            case SDLK_RSHIFT:


                            break;

                            case SDLK_UP:
                                        //si on a appuyé sur la flèche HAUT ...

                                switch(i)
                               {
                                    case 1:
                                        i = 3;  //... et que l'on était sur le bouton 1, alors on se déplace sur bouton 3
                                        btcreer = TTF_RenderText_Blended(policeBt, "Creer partie", couleurBlanche);
                                        btrejoindre = TTF_RenderText_Blended(policeBt, "Rejoindre", couleurBlanche);
                                        btregarder = TTF_RenderText_Blended(policeBt, "Regarder", couleurBleue);
                                    break;

                                    case 2:
                                        i = 1;
                                        btcreer = TTF_RenderText_Blended(policeBt, "Creer partie", couleurBleue);
                                        btrejoindre = TTF_RenderText_Blended(policeBt, "Rejoindre", couleurBlanche);
                                        btregarder = TTF_RenderText_Blended(policeBt, "Regarder", couleurBlanche);
                                    break;

                                    case 3:
                                        i = 2;
                                        btcreer = TTF_RenderText_Blended(policeBt, "Creer partie", couleurBlanche);
                                        btrejoindre = TTF_RenderText_Blended(policeBt, "Rejoindre", couleurBleue);
                                        btregarder = TTF_RenderText_Blended(policeBt, "Regarder", couleurBlanche);
                                    break;
                               }

                            break;

                            case SDLK_DOWN:
                                        //si on a appuyé sur la flèche BAS ...

                                switch(i)
                                {
                                    case 1:
                                        i = 2;  //... et que l'on était sur le bouton 1, alors on se déplace sur bouton 3
                                        btcreer = TTF_RenderText_Blended(policeBt, "Creer partie", couleurBlanche);
                                        btrejoindre = TTF_RenderText_Blended(policeBt, "Rejoindre", couleurBleue);
                                        btregarder = TTF_RenderText_Blended(policeBt, "Regarder", couleurBlanche);
                                    break;

                                    case 2:
                                        i = 3;
                                        btcreer = TTF_RenderText_Blended(policeBt, "Creer partie", couleurBlanche);
                                        btrejoindre = TTF_RenderText_Blended(policeBt, "Rejoindre", couleurBlanche);
                                        btregarder = TTF_RenderText_Blended(policeBt, "Regarder", couleurBleue);
                                    break;

                                    case 3:
                                        i = 1;
                                        btcreer = TTF_RenderText_Blended(policeBt, "Creer partie", couleurBleue);
                                        btrejoindre = TTF_RenderText_Blended(policeBt, "Rejoindre", couleurBlanche);
                                        btregarder = TTF_RenderText_Blended(policeBt, "Regarder", couleurBlanche);
                                    break;
                                }
                            break;

                            case SDLK_RETURN:
                                switch (i){
                                    case 1:
                                        choix = 3;
                                    break;

                                    case 2:
                                        choix = 3;
                                    break;

                                    case 3:
                                        choix = 3;
                                    break;

                                }
                            break;
                        }
                    break;
                }

                // Rafraichissement
                position.x = 0;
                position.y = 0;
                SDL_BlitSurface(fond, NULL, screen, &position);

                position.x = 140;
                position.y = 10;
                SDL_BlitSurface(titre, NULL, screen, &position);

                position.x = 200;
                position.y = 60;
                SDL_BlitSurface(soustitre, NULL, screen, &position);

                position.x = 260;
                position.y = 200;
                SDL_BlitSurface(btcreer, NULL, screen, &position);

                position.x = 260;
                position.y = 280;
                SDL_BlitSurface(btrejoindre, NULL, screen, &position);

                position.x = 260;
                position.y = 360;
                SDL_BlitSurface(btregarder, NULL, screen, &position);

                imageConection = IMG_Load("images/connected.png");
                position.x = LARGEUR-90;
                position.y = HAUTEUR-50;
                SDL_BlitSurface(imageConection, NULL, screen, &position);

                position.x = 20;
                position.y = HAUTEUR-35;
                SDL_BlitSurface(textPseudo, NULL, screen, &position);

                position.x = LARGEUR-300;
                position.y = HAUTEUR-35;
                SDL_BlitSurface(textIp, NULL, screen, &position);

                SDL_Flip(screen);

        }
         else if (choix == 3) {

            /***** AFFICHAGE DU JEU *****/

            SDL_PollEvent(&event);

            switch(event.type)
            {
                    case SDL_QUIT:
                    break;


                    case SDLK_RETURN:
                        choix = 2;
                    break;

                    case SDL_KEYDOWN : // Appuie sur une touche :
                        memset(action, 0,sizeof(action));

                        // On récupère l'état des touches à ce moment la
                        Uint8 *keystates = SDL_GetKeyState(NULL);

                        // Si on a appuyé sur échap on quitte et on réinitialise les variables pour la connexion
                        if(keystates[SDLK_ESCAPE])
                        {
                            sendMessage(sock, "deco");                            
                            choix=1;
                            etatConnection=0;
                            isConnected=0;
                            memset(ip, 0,sizeof(ip));
                            memset(mdp, 0,sizeof(mdp));
                            memset(pseudo, 0,sizeof(pseudo));
                        }
                        // On ajoute les paramètres en fonction des actions voulues
                        if(keystates[SDLK_RIGHT])
                        {
                                 strcat(action,"R");
                        }

                        if(keystates[SDLK_LEFT])
                        {
                                 strcat(action,"L");
                        }
                            
                        if(keystates[SDLK_SPACE])
                        {
                                strcat(action,"J");
                        }

                        if(keystates[SDLK_c])
                        {
                                strcat(action,"F");
                        }
                        if(action[0] > 0)
                        {
                            // On monte la requete
                            memset(str,0,sizeof(str));
                            strcpy(str, action);
                            sprintf(str, "%s:ID%d",str, idPerso);
                            if(!sendMessage(sock, str))
                            {
                                // initClient(ip, 8888, &sock);
                                // if(!sendMessage(sock, str))
                                // {
                                //     printf("Erreur de connexion\n");
                                //     return 0;
                                // }

                            }
                        }
                        break;
                }

               // On attend la socket de retour
                        
               

            // ***** MOUVEMENT SURFACES *****
            tempsActuel = SDL_GetTicks();

            // if ( tempsActuel-tempsPrecedent > 500) {
            //     if (up == 0)
            //         up = 1;
            //     else
            //         up = 0;
            //     tempsPrecedent = tempsActuel; //Le temps "actuel" devient le temps "precedent" pour nos futurs calculs
            // }               


            // // ***** GESTION DES TEXTURES DES SURFACES *****
            //     if (orientation_mario == 0) {
            //         if (jump_mario==1)
            //             mario = IMG_Load("images/mario_right_jump.png");
            //         else if (fire_mario==1)
            //             mario = IMG_Load("images/mario_right_fire.png");
            //         else if (up==0)
            //             mario = IMG_Load("images/mario_right_down.png");
            //         else if (up==1)
            //             mario = IMG_Load("images/mario_right_up.png");
            //     }
            //     else {
            //         if (jump_mario==1)
            //             mario = IMG_Load("images/mario_left_jump.png");
            //         else if (fire_mario==1)
            //             mario = IMG_Load("images/mario_left_fire.png");
            //         else if (up==0)
            //             mario = IMG_Load("images/mario_left_down.png");
            //         else if (up==1)
            //             mario = IMG_Load("images/mario_left_up.png");
            //     }

            //     if (orientation_luigi == 0) {
            //         if (jump_luigi==1)
            //             luigi = IMG_Load("images/luigi_right_jump.png");
            //         else if (fire_luigi==1)
            //             luigi = IMG_Load("images/luigi_right_fire.png");
            //         else if (up==0)
            //             luigi = IMG_Load("images/luigi_right_down.png");
            //         else if (up==1)
            //             luigi = IMG_Load("images/luigi_right_up.png");
            //     }
            //     else {
            //         if (jump_luigi==1)
            //             luigi = IMG_Load("images/luigi_left_jump.png");
            //         else if (fire_luigi==1)
            //             luigi = IMG_Load("images/luigi_left_fire.png");
            //         else if (up==0)
            //             luigi = IMG_Load("images/luigi_left_down.png");
            //         else if (up==1)
            //             luigi = IMG_Load("images/luigi_left_up.png");
            //     }

            // ***** RAFRAICHISSEMENT *****
            frame(screen, fond, sol, plateforme, mario, luigi, posFond, posSol, posPlateforme, posMario, posLuigi, marioFireball, luigiFireball, posMarioFireball, posLuigiFireball);
            SDL_Flip(screen);

        }//if

    }//while


    /******************************** LIBERATION MEMOIRE ********************************/

    SDL_FreeSurface(sol);
    SDL_FreeSurface(plateforme);
    SDL_FreeSurface(mario);
    SDL_FreeSurface(luigi);
    SDL_FreeSurface(marioFireball);
    SDL_FreeSurface(luigiFireball);

    SDL_FreeSurface(btconnecter);
    SDL_FreeSurface(btinscrire);

    SDL_FreeSurface(btcreer);
    SDL_FreeSurface(btrejoindre);
    SDL_FreeSurface(btregarder);
    SDL_FreeSurface(btquitter);

    SDL_FreeSurface(imageConection);
    SDL_FreeSurface(textIp);
    SDL_FreeSurface(textMdp);
    SDL_FreeSurface(textPseudo);
    SDL_FreeSurface(textInfo);

    SDL_FreeSurface(titre);
    SDL_FreeSurface(soustitre);
    SDL_FreeSurface(fond);
    SDL_FreeSurface(screen);

    //On quitte SDL_ttf
    TTF_Quit();

    return EXIT_SUCCESS;

}




