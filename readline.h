#ifndef READLINE_H
#define READLINE_H
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "bool.h"
#define BUFFER_SIZE 2048


struct rl_type {
	char rl_buffer_p[BUFFER_SIZE];//buffer of data read
	size_t rl_buf_size_st; 		//num of bytes in buffer
	int  rl_ind_i;		     //index of next unread char in buffer
	int  rl_fd_i;		     //file-descriptor for file reading from
};

bool readLineBufInit(int,struct rl_type *);
ssize_t readLineBuf(struct rl_type *, char *, size_t);

#endif
