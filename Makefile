all: inetd



daemonize.o: daemonize.h
	gcc -g -c daemonize.c


configs.o : readline.h configs.h
	gcc -g -c configs.c

inetd_poll.o: inet_sockets.h inetd_poll.h
	gcc -g -c inetd_poll.c


inet_sockets.o: inet_sockets.h
	gcc -g -c inet_sockets.c

readline.o: readline.h
	gcc -g -c readline.c

errors.o: errors.h
	gcc -g -c errors.c

inetd: daemonize.o configs.o inetd_poll.o inet_sockets.o readline.o errors.o bool.h
	gcc -g -o inetd inetd.c errors.o daemonize.o configs.o inetd_poll.o inet_sockets.o readline.o


clean:
	rm *.o; rm inetd
