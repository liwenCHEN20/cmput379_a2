#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include "server.h"

char* appName = "server_p";

void runServer(int port, char* docDir, char* logDir)
{
	printf("port: %i\ndocDir: %s\nlogDir: %s\n", port, docDir, logDir);

	int sock, snew, fromlength, number, outnum;

	struct sockaddr_in master, from;

	sock = socket( AF_INET, SOCK_STREAM, 0);
	if( sock < 0 ) {
		perror( "Server: cannot open master socket");
		exit(1);
	}

	master.sin_family = AF_INET;
	master.sin_addr.s_addr = INADDR_ANY;
	master.sin_port = htons(port);

	if(bind(sock, (struct sockaddr*) &master, sizeof(master))) {
		perror( "Server: cannot bind master socket");
		exit(1);
	}

	number = 0;
	listen(sock, 16);

	while(1) {
		fromlength = sizeof(from);
		snew = accept(sock, (struct sockaddr*) &from, &fromlength);
		if(snew < 0) {
			perror("Server: accept failed");
			exit(1);
		}

		char request[512];

		read(snew, request, sizeof(request) - 1);

		printf("Request: \n%s\n\n", request);

//		char response[] = "HTTP/1.1 200 OK\r\n"
//"Content-Type: text/html; charset=UTF-8\r\n\r\n"
//"<doctype !html><html><head><title>Bye-bye baby bye-bye</title>"
//"<style>body { background-color: #111 }"
//"h1 { font-size:4cm; text-align: center; color: black;"
//" text-shadow: 0 0 2mm red}</style></head>"
//"<body><h1>Goodbye, world!</h1></body></html>\r\n";
		char response_400[] = "HTTP/1.1 400 Bad Request\r\n"
"Date: Mon 21 Jan 2008 18:06:16 GMT\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
//"Content-Length: 107\r\n\r\n"
"<doctype !html><html><head><title>Test 123</title><body>"
"<h2>Malformed Request</h2>"
"Your browser sent a request I could not understand."
"</body></html>\r\n";

		char response_400_frompdf[] = "HTTP/1.1 400 Bad Request\r\n"
"Date: Mon 21 Jan 2008 18:06:16 GMT\r\n"
"Content-Type: text/html\r\n"
"Content-Length: 107\r\n\r\n"
"<html><body>\r\n"
"<h2>Malformed Request</h2>\r\n"
"Your browser sent a request I could not understand.\r\n"
"<body></html>\r\n";
		write(snew, response_400_frompdf, sizeof(response_400_frompdf) - 1);
		close(snew);
		
	}

}
