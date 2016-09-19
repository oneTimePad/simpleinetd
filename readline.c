#include "readline.h"
#include <stdio.h>

bool readLineBufInit(int fd_i, struct rl_type *rl_p){
	if(rl_p == NULL){
		errno = EINVAL;
		return FALSE;
	}
	memset(rl_p->rl_buffer_p,0, BUFFER_SIZE);
	rl_p -> rl_fd_i = fd_i;
	rl_p -> rl_ind_i =0;
	rl_p -> rl_buf_size_st = 0;
	return TRUE;
}

ssize_t readLineBuf(struct rl_type *rl_p,char *line, size_t line_size){
	if(rl_p == NULL){
		errno = EINVAL;
		return -1;
	}
	char *start = rl_p -> rl_buffer_p+rl_p -> rl_ind_i;
	int count = 0;
	int num_chars = 0;
	while(*(rl_p -> rl_buffer_p+rl_p->rl_ind_i) !='\n' && count < line_size-1){
		if(rl_p -> rl_ind_i == rl_p -> rl_buf_size_st){
			strncpy(line,start,num_chars);
			line[count] = '\0';
			num_chars = 0;
			line = line+count;
			rl_p -> rl_ind_i = 0;
			ssize_t in;
			switch((in=read(rl_p->rl_fd_i,rl_p->rl_buffer_p,BUFFER_SIZE))){
				case -1:{
					if(errno = EINTR){
						return -2;
					}
					return -1;
					break;
				}
				case 0:{
					return 0;
					break;
				}
				default:{
					rl_p->rl_buf_size_st = in;
					break;
				}

				
			}
		}else{
			num_chars++;
			rl_p->rl_ind_i++;
			count++;
		}
	}
	if(*(rl_p->rl_buffer_p + rl_p -> rl_ind_i) == '\n')	
		rl_p->rl_ind_i++;
	memcpy(line,start,num_chars);
	line[count] = '\0';
	return count;
}

bool readLineClean(struct rl_type *rl_p){
	if(rl == NULL){
		errno = EINVAL;
		return FALSE;
	}

	if(close(rl_p->rl_fd_i) == -1)
		return FALSE;
	return TRUE;



}
