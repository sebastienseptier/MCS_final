
all : clean server main

main : main.c fonctions.c header.h
	gcc fonctions.c main.c `pkg-config --cflags --libs sdl` -g -lpthread -lSDL_image -lSDL_ttf -std=c99 -o main

server : server_co.c server.h
	gcc server.h server_co.c `pkg-config --cflags --libs sdl` -g -lpthread -std=c99 -o server

clean : 
	rm -rf main server

