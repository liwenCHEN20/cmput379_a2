#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include "server.h"
#include "responses.h"
#include "logging.h"

char* appName = "server_p";

void replyToClient(int socketFd);
int readFile(char* fileName, char* data);
char* getHeader(char* request);
char* getRequestedFile(char* request);

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
		char* ip = inet_ntoa(from.sin_addr);
		printf("ip: %s\n", ip);

		char request[512];

		read(snew, request, sizeof(request) - 1);

		printf("Request: \n%s\n\n", request);


		char* requestedFile = getRequestedFile(request);
		printf("File requested: \n%s\n", getRequestedFile(request));

//		char response[] = "HTTP/1.1 200 OK\r\n"
//"Content-Type: text/html; charset=UTF-8\r\n\r\n"
//"<doctype !html><html><head><title>Bye-bye baby bye-bye</title>"
//"<style>body { background-color: #111 }"
//"h1 { font-size:4cm; text-align: center; color: black;"
//" text-shadow: 0 0 2mm red}</style></head>"
//"<body><h1>Goodbye, world!</h1></body></html>\r\n";
/*		char response_400[] = "HTTP/1.1 400 Bad Request\r\n"
"Date: Mon 21 Jan 2008 18:06:16 GMT\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
//"Content-Length: 107\r\n\r\n"
"<doctype !html><html><head><title>Test 123</title><body>"
"<h2>Malformed Request</h2>"
"Your browser sent a request I could not understand."
"</body></html>\r\n";

*/		char response_400_frompdf[] = "HTTP/1.1 400 Bad Request\r\n"
"Date: Mon 21 Jan 2008 18:06:16 GMT\r\n"
"Content-Type: text/html\r\n"
"Content-Length: 107\r\n\r\n"
"<html><body>\r\n"
"<h2>Malformed Request</h2>\r\n"
"Your browser sent a request I could not understand.\r\n"
"<body></html>\r\n";
		char* response400 = getBadRequestResponse();
		int compare = strncmp(response_400_frompdf, response400, 20);
		if(compare != 0) {
			printf("Not equal");
		}
		printf("\nGood:\n\n%s\n\n----------\n\nBad:\n\n%s\n\n----------\n", response_400_frompdf, response400);

		char indexFile[512];
		char fileBuffer[512];

		sprintf(indexFile, "%s%s", docDir, requestedFile);
		int fileLength = readFile(indexFile, fileBuffer);
		printf( "Length: %i\nData: %s\nstrlen(Data): %i\n", fileLength, fileBuffer, strlen(fileBuffer) );

		if( fileLength >= 0) {
			printf( "Response: \n---------\n%s\n-----------\n\n", getValidRequest(fileBuffer));
			char* validResponse = getValidRequest(fileBuffer);

			write(snew, validResponse, strlen(validResponse) - 1);
		}
		else {
			write(snew, response400, strlen(response400) -1);
		}


		char* header = getHeader(request);
		printf("Header: %s\nasdfasdf", header);
		char* header2 = malloc(sizeof(char) * 512);
		sprintf(header2, "%s", header);
		printf("Header2: %s\nasdasdad", header2);

		logErrorMessage(logDir, ip, header2, getHeader(getValidRequest(fileBuffer)));

		close(snew);
	}

}

int readFile(char* fileName, char* data) {
	FILE* file;
	char line[90];
	int fileLength = 0;

	file = fopen(fileName, "rt");
	if(file == NULL) {
		return -1; /* error code for failure to open file */
	}
	while(feof(file) == 0) {
		if(fgets(line, 90, file) != NULL) {
			strcpy(data, line);
			fileLength += strlen(line);
			data += strlen(line);
		}
		else {
			break;
		}
	}
	fclose(file);
	return fileLength;
}

char* getHeader(char* request) {
	char* tempRequest = malloc(sizeof(char) * strlen(request)+1);
	strcpy(tempRequest, request);

	char* header = strsep(&tempRequest, "\r");
	printf("Before newline: --\n%s\n---\nAfter newline: --\n%s\n", header, tempRequest);
	return header;
}
		
char* getRequestedFile(char* request) {
	char* buffer = malloc(sizeof(char) * 512);
	char* get;
	char* http;
	char* file = malloc(sizeof(char) * 256);

	strcpy(buffer, request);

	get = strsep(&buffer, " ");
	if(strncmp(get, "GET", 3) != 0) {
		// error - bad format
	}
	else {
		strcpy(file, strsep(&buffer, " "));
		//http = strsep(&buffer, "\n");
		printf("~~~~\n%s\n~~~~", buffer);
		http = strsep(&buffer, "\n");
		printf("\n--1--\n%s\n--2--\n%s\n--3--\n", http, buffer);
		//strcpy(http, strsep(&buffer, "\n"));
		if(strncmp(http, "HTTP/1.1", 8) != 0) {
			// error - bad format
		}
	}

	return file;
}
