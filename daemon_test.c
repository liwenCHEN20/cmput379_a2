#include <unistd.h>
int main(int argc, char* args[]) {
	daemon(0, 0);
	sleep(30);
}
