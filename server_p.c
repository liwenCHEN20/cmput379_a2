#include <pthread.h>
#include "server.h"
#include <stdio.h>

char* appName = "server_p";

void* pthread_handleRequest(void* arg);

void handleRequest(int sockFd) {
	printf( "in HandleRequest!\n" );
	pthread_t thread;


	printf( "handlerequest id: %i\n", sockFd);
	int* data = malloc(sizeof(int));
	*data = sockFd;
	int error = pthread_create(&thread, NULL, pthread_handleRequest, (void*) data);
	printf("error: %s\n", strerror(error));
	
}

void* pthread_handleRequest(void* arg) {
	printf( "received ptr: %i\n", (int) arg);
	int sockFd = *(int*) arg;
	printf( "handleRequest pthread id: %i\n", sockFd);
	if( sockFd <= 0 ) { // maybe some other condition
		// probably an error
	}
	handleConnection(sockFd);
}
