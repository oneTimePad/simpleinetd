#include "configs.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <errno.h>
#include "readline.h"


static bool parseConfig(char *line, struct service_type *serv){
   	memset(serv,0,sizeof(struct service_type));
   	int order = 0;
   	char *start = line;
	bool end = FALSE;
   	while(TRUE){
		if(*line != ' ' && *line !='\0'){
   			start = (start == NULL) ? line : start;
		
   			line++;
   		}
   		else{
			if(start == NULL && *line!='\0'){
				line++;
				continue;
			}
			else if(start == NULL && *line=='\0'){
				break;
			}
   			switch(order++){
   				case 0:{
   					if(line-start >MAX_SERV_NAME)
   						return FALSE;
   					strncpy(serv->serv_name,start,line-start);
   					serv->serv_name[line-start] = '\0';
   					break;
   				}
				case 1:{
					if(line-start >  MAX_SERV_TYPE)
   						return FALSE;
   					char tmp_type[MAX_SERV_TYPE+1];
   					strncpy(tmp_type,start,line-start);
   					tmp_type[line-start] = '\0';
   					serv->serv_sock_type = (strcmp(tmp_type,"stream")==0)? SOCK_STREAM : ((strcmp(tmp_type,"dgram")==0) ? SOCK_DGRAM : -1);
   					if(serv->serv_sock_type == -1)
   						return FALSE;	
   					break;
				       }
   				case 2:{

					if(line-start >  MAX_SERV_PROT)
   						return FALSE;
   					char tmp_prot[MAX_SERV_PROT+1];
   					strncpy(tmp_prot,start,line-start);
   					tmp_prot[line-start] = '\0';
   					serv->serv_protocol = (strcmp(tmp_prot,"tcp")==0)? SOCK_TCP : ((strcmp(tmp_prot,"udp")==0) ? SOCK_UDP : -1);
   					if(serv->serv_protocol == -1)
   						return FALSE;	
   					break;
				       
				}
   				case 3:{
   					if(line-start >MAX_SERV_FLAG)
   						return FALSE;
   					char tmp_flag[MAX_SERV_FLAG+1];
   					strncpy(tmp_flag,start,line-start);
   					tmp_flag[line-start] = '\0';
   					serv->serv_flag = (strcmp(tmp_flag,"wait")==0) ? WAIT : ((strcmp(tmp_flag,"nowait") ==0) ? NOWAIT : -INVALID);
   					if(serv->serv_flag == INVALID)
   						return FALSE;			
   					break;
   				}
   				case 4:{
   					if(line-start > MAX_SERV_NAME)
   						return FALSE;
   					strncpy(serv->serv_prog,start, line-start);
   					serv->serv_prog[line-start] = '\0';
   					break;
   				}
   				default:{
   					if((long)line-(long)start >ARG_MAX)
   						return FALSE;
   					if(serv->num_args +1 > MAX_SERV_ARGS)
   						return FALSE;
   					strncpy(serv->serv_args[serv->num_args],start,line-start);
   					serv->serv_args[serv->num_args][line-start] = '\0';
					serv->num_args++;
   					break;
   				}
   			}
			if(*line == '\0')
				break;
   			start =NULL;
			line++;
   		}

   
   	}
   	return TRUE;
 	
}

/**
 * initializes reading of a configuration file
 * @config_file: file to open
 * @ rl: ptr to rl_type struct used in readLineBuf(see readLineBuf)
 * returns: status
 */
bool readConfigInit(char * config_file, struct rl_type *rl){
	if(config_file == NULL || rl == NULL){
		errno = EINVAL;
		return FALSE;
	}
	int fd;
	if( (fd = open(config_file,O_RDONLY)) == -1)
		return FALSE;
	if(!readLineBufInit(fd,rl))
		return FALSE;
	return TRUE;
}

bool readConfig(struct rl_type *rl,struct service_type *servs,size_t num_servs,int *start_serv){
	if(rl==NULL || num_servs ==0 || start_serv == NULL || *start_serv <= -1){
		errno = EINVAL;
		return FALSE;
	}
	int curr_serv = *start_serv;
	char line[LINE_SIZE];
	char *line_parse;
	while( readLineBuf(rl,line,LINE_SIZE) > 0){	
		line_parse = line;
		if((rl->rl_buffer_p[rl->rl_ind_i-1]) !='\n'){
			int  extra_size = rl->rl_buf_size_st - rl->rl_ind_i;
			char large[LINE_SIZE+extra_size];
			strcpy(large,line);
			if(readLineBuf(rl,line+extra_size,extra_size) <0)
				return FALSE;
			line_parse = large;
		}
		if(!parseConfig(line_parse,&servs[curr_serv++]))
			return FALSE;
		if(curr_serv >= num_servs){
			*start_serv = curr_serv;
			errno = ENOMEM;
			return FALSE;
		}
	}
	*start_serv = curr_serv;
	return TRUE;
	


}

bool readConfigClean(struct rl_type *rl_p){
	return readLineClean(rl_p);
}

