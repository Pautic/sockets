#include <sys/socket.h>	/* AF_INET */
#include <stdlib.h>	/* strtoul */
#include <math.h>	/* pow() */
#include <stdio.h>		/* perror */

#include "sockets.h"

/* Port */
const unsigned int defport = 5000;
const unsigned int minport = 1024;
const unsigned int maxport = 65537;	/* 65536 + 1 to avoid using <= */

unsigned int get_port(char *argv[])
{
	unsigned int tmp = strtoul(argv[1], NULL, 10);
	return ((tmp > minport) && (tmp < maxport)) ? tmp : defport;
}

/* Domain/family */
const int addrfam = AF_INET;

/* De- and encrypt */
extern unsigned int crypt(unsigned int key, unsigned int msg)
{
	return fmod(pow(msg, (0x00ff & key)), ((key & 0xff00) >> 8));
}

unsigned long long ipow(unsigned long long base, unsigned long long exp)
{
    unsigned long long result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
}
