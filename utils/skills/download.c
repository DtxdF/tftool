/*	download.c
 *
 *	Enviar un archivo del directorio de trabajo al dispositivo que se
 *	desee.
 *
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#include "../../core/get_filesize/get_filesize.h"
#include "../../core/cronometer/cronometer.h"

#include "../interact/interact.h"
#include "../conf_parser/conf_parser.h"
#include "../parser_content/parser_content.h"
#include "../debug/debug.h"
#include "../free_resources/free_resources.h"

void download(struct parser * Parsed, struct filesize * fz,
			  struct client_params * CParams, int * recv_length) {
	FILE * file;

	file = fopen(Parsed->path, "rb");

	if (file != NULL) {
		vprintf("Sending file -> %s", Parsed->path);

		while (!feof(file) && !ferror(file)) {
			*recv_length = fread(Parsed->response, sizeof(char),
								sizeof(Parsed->response), file);
			fz->size += *recv_length;

			get_filesize(fz);

			if (CParams->P_config->debug > 0) {
				vprintf("Sending bytes -> %d - Total:%.3LF%s",
						*recv_length, fz->aux, fz->unit);
				fflush(stdout);

			}

			if (send(CParams->fd, Parsed->response, *recv_length, 0) == -1) {
				eprintf("An error has ocurred sending file \"%s\"",
						Parsed->path);

				break;

			}

		}

		vprintf("Sended -> %.3LF%s", fz->aux, fz->unit);

	} else {
		eprintf("%s: %s", Parsed->path, strerror(errno));
		send_notify("3");
	
	}

	free_resources(CParams->fd, file, Parsed->path, NULL, CParams);

}
