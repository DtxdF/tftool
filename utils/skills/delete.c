/*	delete.c
 *
 *	Borrar un archivo del directorio de trabajo
 *
 */

#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "../../core/cronometer/cronometer.h"

#include "../debug/debug.h"
#include "../interact/interact.h"
#include "../parser_content/parser_content.h"
#include "../free_resources/free_resources.h"

void delete(struct parser * Parsed, struct client_params * CParams) {
	if (unlink(Parsed->path) != 0) {
		eprintf("Error unklinking to \"%s\"", Parsed->path);
		send_notify("5");
	
	} else {
		vprintf("Unlinked -> \"%s\"", Parsed->path);
		send_notify("0");
	
	}

	free_resources(CParams->fd, NULL, Parsed->path, NULL, CParams);

}
