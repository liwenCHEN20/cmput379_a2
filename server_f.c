#include <stdio.h>
#include "server.h"

char* appName = "server_f";

int handleRequest(int oldSockFd, int newSockFd) {
	if(fork() == 0) {
		/* I am the child process */
		close(oldSockFd);
		handleConnection(newSockFd);
		return 1;
	}
	else {
		/* I am the parent process */
		close(newSockFd);
		return 0;
	}

}
