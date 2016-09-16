#include "inetd_poll.h"
#include <sys/sockets.h>
#include <errno.h>




/**
 * initializes sockets to poll on 
 * poll_str: struct pollfd entry
 * serv: service entry
 * returns status
 * */
bool pollInit(struct pollfd *poll_str, service_type *serv){
	if(poll_str == NULL || serv == NULL){
		errno = EINVAL
		return FALSE;
	}

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


static bool pollFind(struct pollfd *poll_strs, nfds_t nfds,struct pollstate_type *ps){


	int i =0;
	for(;i< nfds; i++){
		if(poll_strs[i].revents == POLLIN){
			polls_strs[i].revents =0;
			ps->num_accepted--;
			ps->current_index = i;
			return TRUE;
		}

	}
	return FALSE;
}

/**
 * polls on poll_strs
 * poll_str: list of fd to poll on
 * num_accepted: number of sockets accepted upon last poll
 * returns: status
 * */
bool pollStart(struct pollfd *poll_strs,nfds_t nfds,struct pollstate_type* ps){

	if(poll_str == NULL || ps == NULL || nfds <= 0){
		errno = EINVAL;
		return FALSE;
	}

	#define DFT_TIMEOUT 2000
	if(ps->num_accept>0){
		if(!pollFind(poll,nfds,ps))
			return FALSE;
		return TRUE;
	}
	int ret;	
	while((ret=poll(poll_strs,nfds,DFT_TIMEOUT))==0);
	if(ret == -1)
		return FALSE;
	else{
		if(!pollFind(poll,nfds,ps))
			return FALSE;
		return TRUE;
	}



}
