/*	upload.c
 *
 *	Almacenar un archivo de un dispositivo emisor
 *
*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <pthread.h>

#include "../../core/cronometer/cronometer.h"
#include "../../core/exists/exists.h"
#include "../../core/get_filesize/get_filesize.h"

#include "../debug/debug.h"
#include "../conf_parser/conf_parser.h"
#include "../interact/interact.h"
#include "../parser_content/parser_content.h"
#include "../free_resources/free_resources.h"
#include "../strtok_m/strtok_m.h"
#include "../client/client.h"

void upload(struct parser * Parsed, struct client_params * CParams, 
			struct filesize * fz, int * recv_length) {
	FILE * file;

	if (exists(Parsed->path) == 0) {
		eprintf("The file \"%s\" already exists!", Parsed->path);
		send_notify("4");

		free_resources(CParams->fd, NULL, Parsed->path, NULL, CParams);

		return;
	
	}

	file = fopen(Parsed->path, "wb");
	fz->size = *recv_length;

	if (file == NULL) {
		eprintf("%s: %s", Parsed->path, strerror(errno));
		send_notify("3");

		free_resources(CParams->fd, NULL, Parsed->path, NULL, CParams);

		return;
	
	} else {
		send_notify("0");
	
	}

	vprintf("Saving -> %s (filename:%p, file:%p)",
			Parsed->path, (void *)Parsed->path, file);

	fwrite(Parsed->response, sizeof(char),								/* Guardamos los primeros bytes */
		   strnlen(Parsed->response, sizeof(Parsed->response)), file);	/* parseados. */

	// Ahora todo byte enviado es parte del archivo :p

	while ((*recv_length = recv(CParams->fd, Parsed->response,
							   sizeof(Parsed->response), 0)) > 0) {
		fz->size += *recv_length;
		get_filesize(fz);
		
		if (CParams->P_config->debug > 0){
			vprintf("Received, extra data -> %d - Total:%.3LF%s",
					*recv_length, fz->aux, fz->unit);
			fflush(stdout);

		}

		fwrite(Parsed->response, sizeof(char), *recv_length, file);

	}

	fclose(file); /* Primero cerramos el archivo para que no haya conflicto entre la
					 función "cli_send". */

	cli_send(Parsed->path, CParams->P_config);

	free_resources(CParams->fd, NULL, Parsed->path, NULL, CParams);

}
