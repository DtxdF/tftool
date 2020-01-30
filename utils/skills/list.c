/*	list.c
 *
 *	Listar los archivos del directorio de trabajo
 *
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>

#include "../../core/strlen_m/strlen_m.h"
#include "../../core/cronometer/cronometer.h"

#include "../debug/debug.h"
#include "../conf_parser/conf_parser.h"
#include "../interact/interact.h"
#include "../check_action/check_action.h"
#include "../free_resources/free_resources.h"
#include "../strtok_m/strtok_m.h"

void list(struct client_params * CParams) {
	struct dirent * file_ls;

	char * extension;

	DIR * directory;

	extension = CParams->P_config->extension_only;

	if ((directory = opendir(CParams->P_config->root_folder)) == NULL) {
		eprintf("Error reading the directory: \"%s\"",
				CParams->P_config->root_folder);
		send_notify("2");
		
		free_resources(CParams->fd, NULL, NULL, NULL, CParams);

		return;
	
	}

	while ((file_ls = readdir(directory))) {
		if ((strncmp(file_ls->d_name, ".", sizeof(file_ls->d_name)) != 0) &&
			(strncmp(file_ls->d_name, "..", sizeof(file_ls->d_name)) != 0) &&
			(check_extension(file_ls->d_name, extension, strlen_m(extension)))) {
			if (CParams->P_config->debug > 0) {
				vprintf("Sending \"%s\"", file_ls->d_name);

			}

			if (send(CParams->fd, file_ls->d_name,
				strnlen(file_ls->d_name, sizeof(file_ls->d_name)), 0) == -1) {
				eprintf("Error sending -> \"%s\"", file_ls->d_name);

				break;

			}

			send(CParams->fd, "\n", 1, 0);
		
		}
	
	}

	free_resources(CParams->fd, NULL, NULL, directory, CParams);

}
