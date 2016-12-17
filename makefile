all:
	gcc -o server server.c sockets.c -Wall
	gcc -o client client.c sockets.c -Wall

server:
	gcc -o server server.c sockets.c -Wall

client:
	gcc -o client client.c sockets.c -Wall
