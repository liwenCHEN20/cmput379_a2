#ifndef RESPONSES_H
#define RESPONSES_H
#include "server.h"
extern char* getErrorResponse(eError error);
extern char* getValidResponse(char* content);
#endif /* RESPONSES_H */
