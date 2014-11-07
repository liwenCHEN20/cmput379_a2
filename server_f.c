#include <stdio.h>
#include "server.h"

char* appName = "server_f";

int handleRequest(int sockFd) {
	if(fork() == 0) {
		/* I am the child process */
		handleConnection(sockFd);
		return 1;
	}
	else {
		/* I am the parent process */
		return 0;
	}

}
