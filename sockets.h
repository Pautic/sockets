#ifndef SOCKETS_H
#define SOCKETS_H

/* Port */
extern const unsigned int defport;
extern const unsigned int minport;
extern const unsigned int maxport;

extern unsigned int get_port(char *argv[]);

/* Domain/family */
extern const int addrfam;

#endif
