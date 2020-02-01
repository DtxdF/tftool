#define _CHECK_CONFIG_DEBUG /* Permitir "dprintf" */
#define _CHECK_CONFIG_FUNCS /* Permitir "isempty" y "isincorrect" */

#include "check_config.h"

#include "../exit_secure/exit_secure.h"

void check_config(struct configuration * Config) {
	if (isempty(Config->host)) {
		dprintf("The Host/IP length is invalid");

		exit_s();

	}

	if (isempty(Config->root_folder)) {
		dprintf("The workspace name length is invalid");
	
		exit_s();

	}

	if (isempty(Config->action)) {
		dprintf("The actions length is invalid");
	
		exit_s();

	}

	if (isincorrect(Config->timewait)) {
		dprintf("The time wait number is invalid");
	
		exit_s();

	}	

	return;

}
