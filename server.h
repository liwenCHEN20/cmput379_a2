typedef enum {
	BAD_REQUEST,
	NOT_FOUND,
	FORBIDDEN,
	INTERNAL_ERROR
} eError;
void runServer(int port, char* docDir, char* logDir);
void handleConnection(int sockFd);
