/*	check_action.c
 *
 *	Verifica si una acción es o no coincidente.
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../core/strlen_m/strlen_m.h"
#include "../../core/get_extension/get_extension.h"

#include "../strtok_m/strtok_m.h"
#include "../exit_secure/exit_secure.h"

int cmp_(char * action, char * string, size_t array_length) {
	struct tok Tok;

	char * aux;
	char * token;

	unsigned short int status;
	
	aux = NULL;

	Tok.token = NULL;
	Tok.i = 0;
	Tok.j = 0;

	status = 0;

	if (strncmp(string, "ALL", array_length) == 0) {
		return 1;
	
	}

	aux = (char *)malloc(array_length);

	if (aux == NULL) {
		perror("tftool");
		exit_s();

		return 0;
	
	}

	token = strtok_m(aux, array_length, string, &Tok, ',');

	do {
		if ((token == NULL) && (Tok.token == NULL)) {
			break;
		
		}

		if (strncmp(action, Tok.token, strlen_m(action)) == 0) {
			status = 1;
			break;
		
		}
	
	} while((token = strtok_m(aux, array_length, string, &Tok, ',')));

	if (Tok.token != NULL) {
		if (strncmp(action, Tok.token, strlen_m(action)) == 0) {
			status = 1;
		
		}

	}

	free(aux);

	return status;

}

int check_action(char * action, char * cmp_action, size_t array_length) {
	return(cmp_(action, cmp_action, array_length));

}

int check_extension(char * filename, char * cmp_action, size_t array_length) {
	char * ext;

	ext = get_extension(filename);

	if (ext) {
		return(cmp_(ext, cmp_action, array_length));

	} else {
		return 0;
	
	}

}
