#include <pthread.h>
#include "server.h"

void handleRequest(int sockFd) {
	pthread_t thread;
	pthread_attr_t attr;

	pthread_create(thread, attr, pthread_handleRequest, (void*) sockFd);
	
}

void* pthread_handleRequest(void* param) {
	int sockFd = *((int*) param);
	if( sockFd <= 0 ) { // maybe some other condition
		// probably an error
	}
	handleConnection(sockFd);
}
