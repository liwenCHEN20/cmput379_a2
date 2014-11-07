#ifndef RESPONSES_H
#define RESPONSES_H
#include "server.h"
extern char* getBadRequestResponse();
extern char* getNotFoundResponse();
extern char* getForbiddenResponse();
extern char* getInternalErrorResponse();
extern char* getErrorResponse(eError error);
extern char* getValidRequest(char* content);
#endif /* RESPONSES_H */
