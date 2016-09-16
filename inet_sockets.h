#ifndef INET_SOCKETS_H
#define INET_SOCKETS_H

#include <sys/socket.h>
#include <netdb.h>
#include "bool.h"
#define IS_ADDR_STR_LEN 4096

int inetConnect(const char *, const char *, int );
int inetListen( const char *, int, socklen_t *);
int inetBind(const char *, int, socklen_t *);
char * inetAddressStr(const struct sockaddr *, socklen_t, char *, int);
#endif
