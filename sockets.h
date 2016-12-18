#ifndef SOCKETS_H
#define SOCKETS_H

/* Port */
extern const unsigned int defport;
extern const unsigned int minport;
extern const unsigned int maxport;

extern unsigned int get_port(char *argv[]);

/* Domain/family */
extern const int addrfam;

/* De- and encrypt */
extern unsigned int crypt(unsigned int key, unsigned int msg);

extern unsigned long long ipow(unsigned long long base, unsigned long long exp);

#endif
