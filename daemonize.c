#include <sys/stat.h>
#include <fcntl.h>
#include "daemonize.h"
#include <stdlib.h>
#include <unistd.h>

int daemonize( int flags){

	int maxfd, fd;

	switch(fork()){
		case -1: return -1; break;
		case  0: break;
		default: _exit(EXIT_SUCCESS); break;
	}

	if(setsid() == -1)
		return -1;
	switch(fork()){
		case -1: return -1; break;
		case  0: break;
		default: _exit(EXIT_SUCCESS); break;
	}	
	
	if((flags & DZ_NO_UMASK0))
		umask(0);
	if((flags & DZ_NO_CHDIR))
		chdir("/");

	if((flags & DZ_NO_CLOSE_FILES)){
		maxfd = sysconf(_SC_OPEN_MAX);
		if(maxfd == -1)
			maxfd = DZ_MAX_CLOSE;
		for(fd = 0; fd <maxfd; fd++)
			close(fd);

	}

	if((flags & DZ_NO_REOPEN_STD_FDS)){
		if(close(STDIN_FILENO) == -1)
			return -1;

		fd = open("/dev/null", O_RDWR);
		if(fd != STDIN_FILENO )
			return -1;
		if(dup2(STDIN_FILENO,STDOUT_FILENO) != STDOUT_FILENO)
			return -1;
		if(dup2(STDIN_FILENO,STDERR_FILENO) != STDERR_FILENO)
			return -1;
	}

	return 0;

}
