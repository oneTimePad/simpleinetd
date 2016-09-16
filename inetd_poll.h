#ifndef INET_CON_H
#define INET_CON_H
#include "bool.h"
#include "configs.h"
#include "poll.h"

#define DFT_BACKLOG 5

struct pollstate_type{
	int current_index;
	int num_accepted;
};

bool pollInit(struct pollfd *,struct service_type *);
bool pollAcept(struct service_type *);
bool pollStart(struct pollfd *poll_strs,nfds_t, struct pollstate_type *);


#endif
