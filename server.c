#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <regex.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include "server.h"
#include "responses.h"
#include "logging.h"

char* gDocDir;
char* gLogDir;

void replyToClient(int socketFd);
int readFile(char* fileName, char* data);
char* getHeader(char* request);
char* getRequestedFile(char* request);
void handleConnection(int sockId);
void getIpAddress(int sockId, char* ip);

extern int handleRequest(int oldSockFd, int newSockFd);
void sendErrorResponse(eError error, int sockId, char* responseCode);
int containsValidCharacters(char* fileName);
int getFileLength(char* fileName);

void runServer(int port, char* docDir, char* logDir)
{
	/* makes child processes get automatically cleaned up
	 * since we don't care about what they return.
	 */
	signal(SIGCHLD, SIG_IGN);

	gDocDir = docDir;
	gLogDir = logDir;

	int sock, snew, fromlength, number, outnum;

	struct sockaddr_in master, from;

	sock = socket( AF_INET, SOCK_STREAM, 0);

	/* Following two lines taken from Kirby Banman on the discussion forum */
	int optval = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
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

	//daemon(0, 0);
	
	listen(sock, 16);

	while(1) {
		fromlength = sizeof(from);
		snew = accept(sock, (struct sockaddr*) &from, &fromlength);
		if(snew < 0) {
			perror("Server: accept failed");
			exit(1);
		}
		/* HandleRequest returns 0 for the main process/thread */
		if(handleRequest(sock, snew) != 0)
		{
			break;
		}

	}

}

void handleConnection(int sockId) {
	char clientRequest[512];
	read(sockId, clientRequest, sizeof(clientRequest) - 1);

	char* ip = malloc(sizeof(char) * 100);
	getIpAddress(sockId, ip);

	char* requestHeader = getHeader(clientRequest);

	printf("1\n");
	char* requestedFile = malloc(sizeof(char) * 512);
	int ecode = processRequestHeader(requestHeader, requestedFile);
	char responseCode[64];
	if(ecode || !containsValidCharacters(requestedFile)) {
		printf("BAD_REQUEST\n");
		sendErrorResponse(BAD_REQUEST, sockId, responseCode);
		logErrorMessage(gLogDir, ip, requestHeader, responseCode);
	}
	else {
		char fullPath[512];
		sprintf(fullPath, "%s%s", gDocDir, requestedFile);
		int fileSize = getFileLength(fullPath);
		printf("fileSize: %i\n", fileSize);
		char* fileData = malloc(fileSize + 1);
		int bytesRead = readFile(fullPath, fileData);
		printf("2\n");
		printf("bytesRead: %i\n", bytesRead);
		
		if(bytesRead == -1) {
			printf("NF\n");
			sendErrorResponse(NOT_FOUND, sockId, responseCode);
			logErrorMessage(gLogDir, ip, requestHeader, responseCode);
		}
		else if(bytesRead == -2) {
			printf("F\n");
			sendErrorResponse(FORBIDDEN, sockId, responseCode);
			logErrorMessage(gLogDir, ip, requestHeader, responseCode);
		}
		else if(bytesRead == -3) {
			printf("IE\n");
			sendErrorResponse(INTERNAL_ERROR, sockId, responseCode);
			logErrorMessage(gLogDir, ip, requestHeader, responseCode);
		}
		else {
			printf("VR\n");
			char* validResponse = getValidResponse(fileData);
			int bytesWritten = write(sockId, validResponse, strlen(validResponse));
			char responseCode[64];
			int error = getResponseCode(validResponse, responseCode);
			logSuccessMessage(gLogDir, ip, requestHeader, responseCode, bytesWritten, strlen(validResponse));
			//free(fileData);
			//free(validResponse);
		}
	}
	close(sockId);
}

void sendErrorResponse(eError error, int sockId, char* responseCode) {
	char* response = getErrorResponse(error);
	write(sockId, response, strlen(response));
	getResponseCode(response, responseCode);
}

int getFileLength(char* fileName) {
	FILE* file = fopen(fileName, "rt");
	if(file == NULL) {
		return 0;
	}
	
	int length = 0;
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	fclose(file);
	return length;
}

int readFile(char* fileName, char* data) {
	FILE* file;
	char line[90];
	int fileLength = 0;

	file = fopen(fileName, "rt");
	if(file == NULL) {
		if(errno == ENOENT) {
			return -1; /* file doesn't exist! */
		}
		else if(errno == EACCES) {
			return -2; /* no permissions! */
		}
		else {

			return -3; /* other error. */
		}
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
	return header;
}

int processRequestHeader(char* header, char* requestedFile) {
	char* myHeader = malloc(sizeof(char) * strlen(header) + 1);
	strcpy(myHeader, header);
	
	char* temp = strsep(&myHeader, " ");
	if(strncmp(temp, "GET", 3) != 0) {
		return -1;
	}
	else {
		char* file = strsep(&myHeader, " ");
		if(strncmp(myHeader, "HTTP/1.1", 8) != 0) {
			return -1;
		}
		strcpy(requestedFile, file);
		return 0;
	}
}

int containsValidCharacters(char* fileName) {
	int error;
	regex_t regex;
	
	error = regcomp(&regex, "^[[:alnum:]///._]*$", 0);
	if(error) {
		perror("Regex error");
	}
	
	error = regexec(&regex, fileName, 0, NULL, 0);
	if(error == REG_NOMATCH) {
		return 0;
	}
	return 1;
}

int getResponseCode(char* header, char* responseCode) {
	char* myHeader = malloc(sizeof(char) * strlen(header) + 1);
	strcpy(myHeader, header);

	char* temp = strsep(&myHeader, " ");
	if(strncmp(temp, "HTTP/1.1", 8) != 0) {
		return -1;
	}
	else {
		char* code = strsep(&myHeader, "\r");
		strcpy(responseCode, code);
		return 0;
	}
}

char* getRequestedFile(char* request) {
	char* buffer = malloc(sizeof(char) * 512);
	char* get;
	char* http;
	char* file = malloc(sizeof(char) * 256);

	strcpy(buffer, request);

	get = strsep(&buffer, " ");
	if(strncmp(get, "GET", 3) != 0) {
		/* error - bad format */
	}
	else {
		strcpy(file, strsep(&buffer, " "));
		http = strsep(&buffer, "\n");
		if(strncmp(http, "HTTP/1.1", 8) != 0) {
			// error - bad format
		}
	}

	return file;
}

void getIpAddress(int sockId, char* ip) {
	struct sockaddr_in from;
	socklen_t from_size = sizeof(struct sockaddr_in);
	int err = getsockname(sockId, (struct sockaddr*) &from, &from_size);
	if( err != 0 ) {
		printf("ip didnt work!\n");
		printf("error: %s\n", strerror(errno) );
	}
	char* tempIp = inet_ntoa(from.sin_addr);
	strcpy(ip, tempIp);
}
