#include "configs.h"
#include "daemonize.h"
#include "inetd_poll.h"
#include "errors.h"
#include "inetd.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>

void readConfigServ(char *config_file, struct service_type **servs, size_t *num_servs){
	if(servs == NULL || num_servs == NULL){
		errno = EINVAL;
		errnoExit("readConfigServ");
	}
	struct rl_type rl; //handle for reading lines, see readline.h
	if(!readConfigInit(config_file,&rl))
		errnoExit("readConfigInit");

	size_t local_num_serv = INIT_SERVICE_ARRAY_SIZE;
	struct service_type *local_servs = (struct service_type *)malloc(sizeof(struct service_type) *(local_num_serv));
	if(servs == NULL)
		errnoExit("malloc");
	int start_serv = 0;
	while(!readConfig(&rl, local_servs, local_num_serv,&start_serv)){
		local_servs = realloc(local_servs, sizeof(struct service_type) *(local_num_serv+=INIT_ARRAY_SMALL_INC));
		if(local_servs == NULL)
			errnoExit("realloc");
	}
	*num_servs = start_serv;
	*servs = local_servs;

}

volatile sig_atomic_t hup_received = 0;
void hup_handler(int sig){

	hup_received = 1;

}



int main(int argc,char *argv[]){
	struct sigaction hup_action;
	hup_action.sa_flags = SA_RESTART;
	sigemptyset(&hup_action.sa_mask);
	hup_action.sa_handler = hup_handler;
	if(sigaction(SIGHUP,&hup_action,NULL)  == -1){
		errnoExit("sigaction");

	}

	if(daemonize(0) == -1)
		errExit("daemonize");

	openlog(LOG_IDENF,0,LOG_USER);
	struct service_type *servs = NULL;
	size_t num_servs;
	readConfigServ(CONFIG_FILE,&servs,&num_servs);
	
	struct pollfd poll_servs[num_servs];
	memset(poll_servs,0,num_servs*sizeof(struct pollfd));

	int index = 0;
	for(; index < num_servs; index++){
		if(!pollInit(&poll_servs[index],&servs[index]))
				errnoExit("pollInit");
	}
	struct pollstate_type ps;
	while(!pollStart(poll_servs,num_servs,&ps)){
		
		if(!pollAccept(&servs[ps.current_index],poll_servs[ps.current_index].fd))
			errnoExit("pollAccept");

	}
	errnoExit("pollStart");
	exit(EXIT_SUCCESS);

}



