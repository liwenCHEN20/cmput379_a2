#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "responses.h"

char* getDayOfWeek(int num_day);
char* getMonth(int num_month);

char* getDate() {
	char* date = malloc(sizeof(char) * 512);
	
	time_t curTime = time(NULL);
	struct tm t;

	localtime_r(&curTime, &t);

	sprintf(date,/* ctime(&curTime));*/ "%s %02d %s %d %02d:%02d:%02d %s", getDayOfWeek(t.tm_wday), t.tm_mday,
		 getMonth(t.tm_mon), t.tm_year + 1900, t.tm_hour, t.tm_min, t.tm_sec, "GMT");
	return date;
/*	char* testdate = malloc(sizeof(char) * 512);
	sprintf(testdate, "Mon 21 Jan 2008 18:06:16 GMT");
	return testdate ;*/
}

char* getDayOfWeek(int num_day) {
	char* day = malloc(sizeof(char) * 3);
	switch(num_day) {
		case 0:
			sprintf(day, "Sun");
			break;
		case 1:
			sprintf(day, "Mon");
			break;
		case 2:
			sprintf(day, "Tue");
			break;
		case 3:
			sprintf(day, "Wed");
			break;
		case 4:
			sprintf(day, "Thu");
			break;
		case 5:
			sprintf(day, "Fri");
			break;
		case 6:
			sprintf(day, "Sat");
			break;
		default:
			sprintf(day, "???");
			break;
	}
	return day;
}

char* getMonth(int num_month) {
	char* month = malloc(sizeof(char) * 3);
	switch(num_month) {
		case 0:
			sprintf(month, "Jan");
			break;
		case 1:
			sprintf(month, "Feb");
			break;
		case 2:
			sprintf(month, "Mar");
			break;
		case 3:
			sprintf(month, "Apr");
			break;
		case 4:
			sprintf(month, "May");
			break;
		case 5:
			sprintf(month, "Jun");
			break;
		case 6:
			sprintf(month, "Jul");
			break;
		case 7:
			sprintf(month, "Aug");
			break;
		case 8:
			sprintf(month, "Sep");
			break;
		case 9:
			sprintf(month, "Oct");
			break;
		case 10:
			sprintf(month, "Nov");
			break;
		case 11:
			sprintf(month, "Dec");
			break;
		default:
			sprintf(month, "???");
			break;
	}
	return month;
}

char* getBadRequestResponse() {
	int size = 512;
	char* response = malloc(sizeof(char) * 512);
	sprintf(response, "HTTP/1.1 400 Bad Request\r\nDate: %s\r\nContent-Type: text/html\r\n"
		"Content-Length: 107\r\n\r\n<html><body>\r\n<h2>Malformed Request</h2>\r\n"
		"Your browser sent a request I could not understand.\r\n"
		"<body></html>\r\n", getDate());
	return response;
}

char* getNotFoundResponse() {
	int size = 512;
	char* response = malloc(sizeof(char) * 512);
	sprintf(response, "HTTP/1.1 404 Not Found\r\nDate: %s\r\nContent-Type: text/html\r\n"
	"Content-Length: 117\r\n\r\n"
	"<html><body>\r\n<h2>Document not found</h2>\r\n"
	"You asked for a document that doesn't exist. That is so sad.\r\n"
	"<body></html>\r\n", getDate());
	return response;
}

char* getForbiddenResponse() {
	int size = 512;
	char* response = malloc(sizeof(char) * 512);
	sprintf(response, "HTTP/1.1 403 Forbidden\r\n"
		"Date: %s\r\n"
		"Content-Type: text/html\r\n"
		"Content-Length: 130\r\n\r\n"
		"<html><body>\r\n<h2>Permission Denied</h2>\r\n"
		"You asked for a document you are not permitted to see. It sucks to be you.\r\n"
		"</body></html>\r\n", getDate());
	return response;
}

char* getInternalErrorResponse() {
	int size = 512;
	char* response = malloc(sizeof(char) * 512);
	sprintf(response, "HTTP/1.1 500 Internal Server Error\r\n"
		"Date: %s\r\n"
		"Content-Type: text/html\r\n"
		"Content-Length: 131\r\n\r\n"
		"<html><body>\r\n<h2>Oops. That didn't work</h2>\r\n"
		"I had some sort of problem dealing with your request. Sorry, I'm lame.\r\n"
		"</body></html>/r/n", getDate());
	return response;
}
	


char* getValidRequest(char* content) {
	int length = strlen(content);

	char* response = malloc(sizeof(char) * 512);
	sprintf(response, "HTTP/1.1 200 OK\r\nDate: %s\r\nContent-Type: text/html\r\n"
		"Content-Length: %i\r\n\r\n%s\r\n", getDate(), length, content);

	return response;
}
