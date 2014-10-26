#include <stdio.h>
#include "server.h"

char* appName = "server_f";

void runServer(int port, char* docDir, char* logDir)
{
	printf("port: %i\ndocDir: %s\nlogDir: %s\n", port, docDir, logDir);
}
