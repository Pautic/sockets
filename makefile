all:
	gcc -o server server.c -Wall
	gcc -o client client.c -Wall

server:
	gcc -o server server.c -Wall

client:
	gcc -o client client.c -Wall
