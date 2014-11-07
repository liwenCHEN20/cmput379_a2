#include <pthread.h>
#include <stdlib.h>
#include "server.h"
#include <stdio.h>

char* appName = "server_p";

void* pthread_handleRequest(void* arg);

int handleRequest(int oldSockFd, int newSockFd) {
	pthread_t thread;

	int* data = malloc(sizeof(int));
	*data = newSockFd;
	int error = pthread_create(&thread, NULL, pthread_handleRequest, (void*) data);
	return 0;
	
}

void* pthread_handleRequest(void* arg) {
	int sockFd = *(int*) arg;
	if( sockFd <= 0 ) { // maybe some other condition
		// probably an error
	}
	handleConnection(sockFd);
}
