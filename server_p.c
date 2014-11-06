#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include "server.h"

char* appName = "server_p";

void runServer(int port, char* docDir, char* logDir)
{
	printf("port: %i\ndocDir: %s\nlogDir: %s\n", port, docDir, logDir);
	int socketId = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = port;
	address.sin_addr = (struct in_addr) INADDR_LOOPBACK;

	if(bind(socketId, (struct sockaddr*) &address, sizeof(sockaddr_in)) == -1)
	{
		/* error */
	}
	else
	{
		if(listen(socketId, 50) == -1)
		{
			/* error */
		}
		else
		{
			
		}
	}

}
