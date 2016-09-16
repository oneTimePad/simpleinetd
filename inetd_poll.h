#ifndef INET_CON_H
#define INET_CON_H
#include "bool.h"
#include "configs.h"




bool pollInit(struct pollfd *,struct service_type *);
bool pollAcept(struct service_type *);
bool pollStart(struct pollfd *poll_strs,int *);


#endif
