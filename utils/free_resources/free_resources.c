/*	free_resources.c
 *
 *	Facilita la vida. Cierra y/o libera varios tipos de recursos.
 *
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

#include "../../core/cronometer/cronometer.h"
#include "../interact/interact.h"

#include "../debug/debug.h"

void free_resources(int fd, void * file, void * heap_addr, void * dir,
					struct client_params * CParams) {
	cprintf(stderr, "Freeing up resources...");

	if (fd > -1) {
		if (close(fd) != 0) {
			cprintf(stderr, "Error closing file descriptor \"%d\". ErrorCode:%d - ErrorMSG:%s",
					fd, errno, strerror(errno));

		} else {
			cprintf(stderr, "Closed, file descriptor \"%d\"", fd);

		}
	
	}

	if (file != NULL) {
		if (fclose(file) != 0) {
			cprintf(stderr, "Error closing file with address \"%p\"", file);
		
		} else {
			cprintf(stderr, "Closed, file with address \"%p\"", file);

		}
	
	}

	if (dir != NULL) {
		if (closedir(dir) != 0) {
			cprintf(stderr, "Error closing directory with address \"%p\"", dir);
		
		} else {
			cprintf(stderr, "Closed, directory with address \"%p\"", dir);

		}
	
	}

	if (heap_addr != NULL) {
		cprintf(stderr, "Freeing data on the heap \"%p\"", heap_addr);
		free(heap_addr);
		
	}

	if (CParams != NULL) {
		*CParams->n_client -= 1;
		CParams->init = 0;
	
	}

}
