#include "configs.h"
#include "errors.h"



volatile sig_atomic_t hup_received = 0;
void hup_handler(int sig){

	hup_received = 1;

}



int main(int argc,char *argv[]){
	struct sigaction hup_action;
	hup_action.flags = SA_RESTART;
	sigemptyset(&hup_action.sa_mask);
	hup_action.handler = hup_handler;
	if(sigaction(SIGHUP,&hup_action,NULL)  == -1){
		errnoExit("sigaction");

	}

	if(daemonize(0) == -1)
		errExit("daemonize");

	openlog(LOG_IDENF,0,LOG_USER);
	struct rl_type rl; //handle for reading lines, see readline.h
	if(!readConfigInit(CONFIG_FILE,&rl))
		errnoExit("readConfigInit");

	size_t num_services = INIT_SERVICE_ARRAY_SIZE;
	struct service_type *servs = (struct service_type *)malloc(sizeof(struct service_type) *(size+=INIT_ARRAY_SMALL_INC));
	if(servs == NULL)
		errnoExit("malloc");

	if(!readConfig(&rl))
		errnoExit("readConfig");

}
