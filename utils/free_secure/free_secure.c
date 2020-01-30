/*	free_secure.c
 *
 *	Libera los recursos antes de salir
 *
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "../../core/cronometer/cronometer.h"

#include "../interact/interact.h"
#include "../debug/debug.h"
#include "../conf_parser/conf_parser.h"

void free_secure(struct client_params * CParams, struct configuration * P_config) {
	static struct client_params * Params;
	static struct configuration * Config;

	if ((CParams != NULL) && (P_config != NULL)) {
		Params = CParams;
		Config = P_config;
	
	} else {
		free(Params);
		free(Config->root_folder);
		free(Config->extension_only);
		free(Config->backup);

		pthread_mutex_destroy(Config->mutex);
	
	}

}

void sig_secure(int signum) {
	signal(signum, SIG_IGN);

	free_secure(NULL, NULL);

	dprintf("Terminate.");

	signal(signum, sig_secure);

	pthread_exit(0);

	exit(EXIT_SUCCESS);

}
