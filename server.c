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

char* appName = "server";
char* gDocDir;
char* gLogDir;

void replyToClient(int socketFd);
int readFile(char* fileName, char* data);
char* getHeader(char* request);
char* getRequestedFile(char* request);
void handleConnection(int sockId);
void getIpAddress(int sockId, char* ip);

void runServer(int port, char* docDir, char* logDir)
{
	printf("port: %i\ndocDir: %s\nlogDir: %s\n", port, docDir, logDir);

	char test[] = "12345";
	printf("strlen(test): %i\n", strlen(test));

	gDocDir = docDir;
	gLogDir = logDir;

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
		handleConnection(snew);

	}

}

void handleConnection(int sockId) {
	char clientRequest[512];
	read(sockId, clientRequest, sizeof(clientRequest) - 1);

	char* ip = malloc(sizeof(char) * 100);
	getIpAddress(sockId, ip);
	printf("IP: \n%s\n", ip);

	char* requestHeader = getHeader(clientRequest);
	printf("request: \n%s\n", clientRequest);
	printf("request header: \n%s\n", requestHeader);
	char* requestedFile = malloc(sizeof(char) * 512);
	int ecode = processRequestHeader(requestHeader, requestedFile);
	printf("requested file: \n%s\n", requestedFile);
	if(ecode) {
		// 400 bad request
		printf("400 bad request\n");
	}
	else {
		char fileData[512];
		char fullPath[512];
		sprintf(fullPath, "%s%s", gDocDir, requestedFile);
		int fileSize = readFile(fullPath, fileData);
		if(fileSize == -1) {
			// 404 file not found
			printf("404 File not Found\n");
		}
		else if(fileSize == -2) {
			// 403 forbidden
			printf("403 forbidden\n");
		}
		else if( fileSize == -3) {
			// 500 internal error
			printf("500 internal error\n");
		}
		else {
			printf("file data: \n%s\n", fileData);
			// 200 ok
			char* validResponse = getValidRequest(fileData);
			printf("response: \n%s\n", validResponse);
			int bytesWritten = write(sockId, validResponse, strlen(validResponse));
			char responseCode[64];
			int error = getResponseCode(validResponse, responseCode);
			printf("responsecode: \n%s\n", responseCode);
			printf("bytes written %i\nsize of response: %i\n", bytesWritten, strlen(validResponse));
			logSuccessMessage(gLogDir, ip, requestHeader, responseCode, bytesWritten, strlen(validResponse));
		}
	}
	close(sockId);
}

int readFile(char* fileName, char* data) {
	FILE* file;
	char line[90];
	int fileLength = 0;

	file = fopen(fileName, "rt");
	if(file == NULL) {
		if(errno == ENOENT) {
			return -1; // file doesn't exist!
		}
		else if(errno == EACCES) {
			return -2; // no permissions!
		}
		else {

			return -3; // other error.
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
	//printf("Before newline: --\n%s\n---\nAfter newline: --\n%s\n", header, tempRequest);
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

int getResponseCode(char* header, char* responseCode) {
	char* myHeader = malloc(sizeof(char) * strlen(header) + 1);
	strcpy(myHeader, header);

	char* temp = strsep(&myHeader, " ");
	printf("temp: \n%s\n", temp);
	printf("myHeader: \n%s\m", myHeader);
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
		// error - bad format
	}
	else {
		strcpy(file, strsep(&buffer, " "));
		//http = strsep(&buffer, "\n");
	//	printf("~~~~\n%s\n~~~~", buffer);
		http = strsep(&buffer, "\n");
	//	printf("\n--1--\n%s\n--2--\n%s\n--3--\n", http, buffer);
		//strcpy(http, strsep(&buffer, "\n"));
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
	printf("%i, %i\n", from.sin_family, AF_INET);
	char* tempIp = inet_ntoa(from.sin_addr);
	printf("tempip: %s\n", tempIp);
	strcpy(ip, tempIp);
}
