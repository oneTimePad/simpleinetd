#ifndef DAEMONIZE_H
#define DAEMONIZE_H


#define DZ_NO_CHDIR 	      1 // don't chdir("/")
#define DZ_NO_CLOSE_FILES     2 // don't close all fd
#define DZ_NO_REOPEN_STD_FDS  4 // don't reopen stdfd's to /dev/null


#define DZ_NO_UMASK0          10 // don't do umask(0)



#define DZ_MAX_CLOSE          8192    //maximum file descriptors to close


int daemonize(int);
#endif
