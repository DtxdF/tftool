/*	parser_content.c
 *
 *	Parsea el contenido enviado.
 *
*/

#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "../interact/interact.h"
#include "parser_content.h"
#include "../free_resources/free_resources.h"
#include "../debug/debug.h"
#include "../conf_parser/conf_parser.h"
#include "../strtok_m/strtok_m.h"
#include "../check_action/check_action.h"
#include "../exit_secure/exit_secure.h"

#include "../../core/join_filename/join_filename.h"
#include "../../core/cronometer/cronometer.h"
#include "../../core/strlen_m/strlen_m.h"
#include "../../core/isdir/isdir.h"

int parse_content(struct parser * Parsed, struct client_params * CParams) {
	int path_length;

	strncpy(Parsed->cmd, Parsed->content, sizeof(char) * 2); // Parseamos el comando

	if ((MATCH_P("d:")) || (MATCH_P("u:")) || (MATCH_P("D:"))) {
		strncpy(Parsed->filename, Parsed->content + 2, sizeof(Parsed->filename)); // Ahora el nombre del archivo

		// Se calcula la longitud del path

		path_length = sizeof(char) * (4 + strnlen(Parsed->filename, sizeof(Parsed->filename)) + \
									  strnlen(CParams->P_config->root_folder,
											  strlen_m(CParams->P_config->root_folder))) + 2;
		Parsed->path = (char *)malloc(path_length);

		if (Parsed->path == NULL) {
			perror("tftool");
			exit_s();

			return -1;

		}

		join_filename(Parsed->path, CParams->P_config->root_folder,
					  basename(Parsed->filename), path_length);

		if (isdir(Parsed->path) == 1) {
			cprintf(stderr, "%s is a directory!",
					Parsed->path);
			free(Parsed->path);
			return -1;
		
		}

		// Se verifica que la extensión esté permitida
	
		if (!check_extension(Parsed->path, CParams->P_config->extension_only,
							 strlen_m(CParams->P_config->extension_only))) {
			cprintf(stderr, "The \"%s\" file has an incorrect extension", Parsed->path);
			free(Parsed->path);

			return -1;

		}

	}

	if (MATCH_P("u:")) {
		// Se calcula el contenido del archivo

		strncpy(Parsed->response, Parsed->content + (3 + strnlen(
				Parsed->filename, sizeof(Parsed->filename))), sizeof(Parsed->response));
	
	}

	return 0;

}
