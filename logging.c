#include "logging.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void logToFile(char* logFile, char* message);

void logErrorMessage(char* logFile, char* clientIp, char* request, char* responseCode) {
	char* date = getDate();
	char logMessage[strlen(date)+strlen(clientIp)+strlen(request)+strlen(responseCode)+5];

	sprintf(logMessage, "%s\t%s\t%s\t%s\n", date, clientIp, request, responseCode);

	printf("Message:\n%s\n--------\n", logMessage);
	logToFile(logFile, logMessage);
}

void logSuccessMessage(char* logFile, char* clientIp, char* request, char* responseCode, int bytesWritten, int responseLength) {
	char* date = getDate();
	char logMessage[strlen(date)+strlen(clientIp)+strlen(request)+strlen(responseCode)+20];

	sprintf(logMessage, "%s\t%s\t%s\t%s %i/%i\n", date, clientIp, request, responseCode, bytesWritten, responseLength);

	printf("Message:\n%s\n---------\n", logMessage);
	logToFile(logFile, logMessage);
}

void logToFile(char* logFile, char* message) {

	FILE* file;
	FILE* start;
	int error;
	int timeout = 0;

	file = fopen(logFile, "wx");

	if(file)
		fclose(file);

	file = fopen(logFile, "rb+");

	start = file;
	rewind(start);
	error = lockf(fileno(start), F_TLOCK, 0);
	while( error == EACCES || error == EAGAIN)
	{
		usleep(200);
		timeout += 200;
		if( timeout > 5000 ) {
			return;
		}

		error = lockf(fileno(start), F_TLOCK, 0);
	}

	fseek(file, 0, SEEK_END);
	fwrite((const void*) message, 1, strlen(message), file);

	rewind(start);
	fflush(file);
	lockf(fileno(start), F_ULOCK, 0);
	fclose(file);
	

}
