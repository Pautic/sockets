#include "sockets.h"

const unsigned int defport = 5000;
const unsigned int minport = 1024;
/* 65536 + 1 to avoid using <= */
const unsigned int maxport = 65537;
