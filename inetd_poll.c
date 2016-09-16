#include "inetd_poll.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <netdb.h>
#include "inet_sockets.h"


/**
 * initializes sockets to poll on 
 * poll_str: struct pollfd entry
 * serv: service entry
 * returns status
 * */
bool pollInit(struct pollfd *poll_str, struct service_type *serv){
	if(poll_str == NULL || serv == NULL){
		errno = EINVAL;
		return FALSE;
	}

	socklen_t addrlen;
	int fd;
	if(serv->serv_protocol == SOCK_TCP){
		if((fd=inetListen(serv->serv_name,DFT_BACKLOG,&addrlen))==-1)
			return FALSE;
		poll_str->fd = fd;
		poll_str->events = POLLIN;
		poll_str->revents = 0;
		
	}
	else if(serv->serv_protocol == SOCK_UDP){
		if((fd=inetBind(serv->serv_name,SOCK_DGRAM,&addrlen)) ==-1)
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
	struct sockaddr_in client;
	int asockfd;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	if(serv->serv_sock_type == SOCK_STREAM)
		asockfd = accept(sock,(struct sockaddr *)&client, &addrlen);
	else
		asockfd =-1;
	pid_t pid;
	switch((pid =fork())){
		case -1:{
			return FALSE;
			break;
		}
		case 0:{
			if(dup2((asockfd==-1)? sock : asockfd,STDIN_FILENO) == -1)
				return FALSE;
			if(dup2((asockfd == -1)? sock: asockfd,STDOUT_FILENO) == -1)
				return FALSE;
			if(dup2((asockfd == -1)? sock: asockfd,STDERR_FILENO) == -1)
				return FALSE;
			char *args[MAX_SERV_ARGS+2];
			args[0] = serv->serv_prog;
			int i =1;
			for(;i<serv->num_args;i++){
				args[i] = serv->serv_args[i-1];
			}
			args[i] = NULL;


			execv(args[0], args);
			perror("execv");
			exit(EXIT_FAILURE);
			break;
		}
		default:{
			if(asockfd != -1){
				if(close(asockfd) == -1)
					return FALSE;
			}
			if(serv->serv_flag == WAIT){
				int status;
				waitpid(pid,&status,WUNTRACED);
				break;
			}
			break;

		}
	}
	return TRUE;



}


static bool pollFind(struct pollfd *poll_strs, nfds_t nfds,struct pollstate_type *ps){


	int i =0;
	for(;i< nfds; i++){
		if(poll_strs[i].revents == POLLIN){
			poll_strs[i].revents =0;
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
bool pollStart(struct pollfd *poll_strs,nfds_t nfds,struct pollstate_type *ps){

	if(poll_strs == NULL || ps == NULL || nfds <= 0){
		errno = EINVAL;
		return FALSE;
	}

	#define DFT_TIMEOUT 2000
	if(ps->num_accepted>0){
		if(!pollFind(poll_strs,nfds,ps))
			return FALSE;
		return TRUE;
	}
	int ret;	
	while((ret=poll(poll_strs,nfds,DFT_TIMEOUT))==0);
	if(ret == -1)
		return FALSE;
	else{
		if(!pollFind(poll_strs,nfds,ps))
			return FALSE;
		return TRUE;
	}



}
