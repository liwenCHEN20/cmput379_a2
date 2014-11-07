#include "logging.h"
#include <stdio.h>
#include <string.h>

void logErrorMessage(char* logFile, char* clientIp, char* request, char* responseCode) {
	char* date = getDate();
	char logMessage[strlen(date)+strlen(clientIp)+strlen(request)+strlen(responseCode)+5];

	sprintf(logMessage, "%s\t%s\t%s\t%s\n", date, clientIp, request, responseCode);

	printf("Message:\n%s\n--------\n", logMessage);
}

void logSuccessMessage(char* logFile, char* clientIp, char* request, char* responseCode, int bytesWritten, int responseLength) {
	char* date = getDate();
	char logMessage[strlen(date)+strlen(clientIp)+strlen(request)+strlen(responseCode)+20];

	sprintf(logMessage, "%s\t%s\t%s\t%s %i/%i\n", date, clientIp, request, responseCode, bytesWritten, responseLength);

	printf("Message:\n%s\n---------\n", logMessage);
}
