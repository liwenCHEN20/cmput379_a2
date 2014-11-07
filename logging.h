#ifndef LOGGING_H
#define LOGGING_H
extern void logErrorMessage(char* logFile, char* clientIp, char* request, char* responseCode);
extern void logSuccessMessage(char* logFile, char* clientIp, char* request, char* responseCode, int bytesWritten, int responseLength);
#endif /* LOGGING_H */
