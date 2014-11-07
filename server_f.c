#include <stdio.h>
#include "server.h"

char* appName = "server_f";

void handleRequest(int sockFd) {
	if(fork() <= 0) {
		return;
	}
	else {
		handleConnection(sockFd);
	}

}
