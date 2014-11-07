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
	char* error;

	int port = atol(args[1]);

	char* docDir = args[2];
	char* logFile = args[3];

	runServer(port, docDir, logFile);
}

void usage()
{
	printf("Usage:\n%s <port number> <doc directory> <log file>\n", appName);
	printf("Note: Use absolute paths for the directories.\n");
}
