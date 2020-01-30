/*	exists.c
 *
 *	Verifica si un archivo existe
 *
 */

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

int exists(char * filename) {
	int fd;

	fd = open(filename, O_RDONLY);

	if (fd < 0) {
	    return (errno == ENOENT) ? -1 : -2;
	
	}

	close(fd);

	return 0;

}
