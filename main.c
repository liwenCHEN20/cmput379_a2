#include <stdio.h>
#include <stdlib.h>
#include "server.h"

extern char* appName;
void usage();

int main(int argc, char* args[])
{
	if(argc != 4)
	{
		usage();
		return -1;
	}
	int port = atoi(args[1]);
	char* docDir = args[2];
	char* logDir = args[3];

	/* do some validation, display usage if bad data */
	runServer(port, docDir, logDir);
}

void usage()
{
	printf("Usage:\n%s <port number> <doc directory> <log directory>\n", appName);
}
