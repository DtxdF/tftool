#include <signal.h>
#include <unistd.h>

void exit_s(void) {
	kill(getpid(), SIGTERM);

}
