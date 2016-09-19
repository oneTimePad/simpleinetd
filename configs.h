#ifndef CONFIGS_H
#define CONFIGS_H

#include <unistd.h>
#include "bool.h"
#include "readline.h"
#ifndef ARG_MAX
#define ARG_MAX 1024
#endif

#define MAX_SERV_NAME 4096
#define MAX_SERV_ARGS 10
#define MAX_SERV_PROT 10
#define MAX_SERV_FLAG 10
#define MAX_SERV_TYPE 10

#define LINE_SIZE     4096
#define INIT_SERVICE_ARRAY_SIZE 32
#define INIT_ARRAY_SMALL_INC   16
typedef enum {SOCK_TCP, SOCK_UDP} protocol;
typedef enum {INVALID,NOWAIT, WAIT} inet_flags;

struct service_type{
	char serv_name[MAX_SERV_NAME+1];
	long serv_sock_type;
	protocol serv_protocol;
	inet_flags serv_flag;
	char serv_prog[MAX_SERV_NAME+1];
	char serv_args[MAX_SERV_ARGS][ARG_MAX];
	int num_args;

};
		
bool readConfigInit(char *, struct rl_type *);
bool readConfig(struct rl_type *,struct service_type *,size_t,int*);
bool readConfigClean(struct rl_type *);
#endif
