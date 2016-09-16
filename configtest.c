#include "configs.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>






int 
main( int argc, char *argv[]){

char *file = "tc.conf";
struct rl_type rl;
if(!readConfigInit(file,&rl)){
	perror("readConfigInit");
	exit(EXIT_FAILURE);
}
#define NUM_SERVS 1024
struct service_type *servs = (struct service_type *)malloc(sizeof(struct service_type) * NUM_SERVS);
if(servs == NULL){
	perror("malloc");
	exit(EXIT_FAILURE);
}
int index_servs =0;
size_t size = NUM_SERVS;
while(!readConfig(&rl,servs,size,&index_servs)){
	if(errno!=ENOMEM){
		perror("readConfig");
		exit(EXIT_FAILURE);
	}
	servs = (struct service_type *)realloc(servs,sizeof(struct service_type)*(size+10));
	if(servs == NULL){
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	size+=10;
	
}


exit(EXIT_SUCCESS);

}

