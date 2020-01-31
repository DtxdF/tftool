#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int redirection(char * out) {
	int fd;

	fd = open(out, O_CREAT | O_WRONLY | O_APPEND);

	if (fd == -1) {
		return -1;
	
	}

	dup2(fd, 1);
	dup2(fd, 2);

	return fd;

}
