#include "inetd_poll.h"
#include <sys/sockets.h>
#include <poll.h>




/**
 * initializes sockets to poll on 
 * poll_str: struct pollfd entry
 * serv: service entry
 * returns status
 * */
bool pollInit(struct pollfd *poll_str, service_type *serv){

	socklen_t addrlen;
	int fd;
	if(serv->protcol == SOCK_TCP){
		if((fd=inetListen(serv->serv_name,DFT_BACKLOG,&addrlen))==-1)
			return FALSE;
		poll_str->fd = fd;
		poll_str->events = POLLIN;
		poll_str->revents = 0;
		
	}
	else if(serv->protocol == SOCK_UDP){
		if((fd=inetBind(serv->serv_name,&addrlen) ==-1)
				return FALSE;
		poll_str->fd = fd;
		poll_str->events = POLLIN;
		poll_str->revents = 0;
	}
	
	return TRUE;


}

/**
 * execs program associated with `sock`
 * serv: service entry associated with program
 * sock: socket fd associated with program
 * returns: status
 * */
bool pollAccept(struct service_type *serv,int sock){
}

/**
 * polls on poll_strs
 * poll_str: list of fd to poll on
 * poll_index: returns accepted fd 
 * returns: status
 * */
bool pollStart(struct pollfd *poll_strs,int *poll_index){



}
