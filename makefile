all:
	gcc -o server server.c sockets.c -Wall -lm
	gcc -o client client.c sockets.c -Wall -lm

server:
	gcc -o server server.c sockets.c -Wall -lm

client:
	gcc -o client client.c sockets.c -Wall -lm
