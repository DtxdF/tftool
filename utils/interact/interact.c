/*	interact.c
 *
 *	Es el núcleo de tftool y tiene el objetivo de interactuar con los
 *	clientes.
 *
 */

#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <stdlib.h>
#include <libgen.h>
#include <signal.h>

#include "../../conf/config.h"
#include "../../core/exists/exists.h"
#include "../../core/cronometer/cronometer.h"
#include "../../core/get_filesize/get_filesize.h"

#include "interact.h"
#include "../conf_parser/conf_parser.h"
#include "../debug/debug.h"
#include "../free_resources/free_resources.h"
#include "../free_secure/free_secure.h"
#include "../parser_content/parser_content.h"
#include "../exit_secure/exit_secure.h"
#include "../check_action/check_action.h"
#include "../skills/tools.h"

#define CMP_ACTION(c) check_action(c, CParams->P_config->action, sizeof(CParams->P_config->action))

void * thread_interact(void * args) {
	struct client_params * CParams;
	struct filesize fz;
	struct parser Parsed;

	int recv_length;

	fz.size = 0;
	CParams = (struct client_params *)args;

	// Por si otro hilo quiere liberarlo al mismo tiempo
	
	pthread_mutex_lock(CParams->P_config->mutex);

	free(CParams->P_config->root_folder);
	free(CParams->P_config->extension_only);
	free(CParams->P_config->backup);

	parser(CParams->P_config);

	pthread_mutex_unlock(CParams->P_config->mutex);

	vprintf("Receiving ...");

	if ((recv_length = recv(CParams->fd, Parsed.content,
							sizeof(Parsed.content), 0)) <= 0) {
		eprintf("Error receiving data");

		free_resources(CParams->fd, NULL, NULL, NULL, CParams);
		
		return NULL;

	}

	vprintf("%d Byte's Received!", recv_length);

	Parsed.content[recv_length] = '\0';

	if (parse_content(&Parsed, CParams) == -1) {
		eprintf("Error in the actions!");
		send_notify("5");

		free_resources(CParams->fd, NULL, NULL, NULL, CParams);

		return NULL;

	}

	vprintf("CMD -> %s", Parsed.cmd);

	if ((MATCH("d:")) && (CMP_ACTION("download"))) {
		download(&Parsed, &fz, CParams, &recv_length);

	} else if ((MATCH("l:")) && ((CMP_ACTION("list")))) {
		list(CParams);
	
	} else if ((MATCH("u:")) && (CMP_ACTION("upload"))) {
		upload(&Parsed, CParams, &fz, &recv_length);

	} else if ((MATCH("D:")) && (CMP_ACTION("delete"))) {
		delete(&Parsed, CParams);

	} else {
		eprintf("The command \"%s\" is not available!", Parsed.cmd);
		send_notify("1");
		free_resources(CParams->fd, NULL, NULL, NULL, CParams);
	
	}

	return NULL;

}

void interact(int fd, struct configuration * P_config) {
	struct sockaddr_in Client;
	struct client_params * Params;

	register int i;
	int n_client;

	pthread_mutex_t mutex;
	socklen_t sin_size;

	P_config->mutex = &mutex;
	n_client = 0;
	sin_size = sizeof(struct sockaddr);
	Params = (struct client_params *)malloc(sizeof(struct client_params) * P_config->users_limit);

	if (!Params) {
		cprintf(stderr, "Error: %s", strerror(errno));
		exit(EXIT_FAILURE);
	
	}

	if (exists(P_config->root_folder) != 0) {
		cprintf(stderr, "Error, The root directory is not available. ErrorCode:%d - ErrorMsg:%s",
				errno, strerror(errno));
		exit_s();
	
	}

	if ((P_config->users_limit < 2) || P_config->users_limit > MAX_USER_LIMITS) {
		cprintf(stderr, "The limits of connected users must be greater than 2 or less than %d",
				MAX_USER_LIMITS);
		exit_s();
	
	}

	if (pthread_mutex_init(&mutex, NULL) != 0) {
		cprintf(stderr, "Error creating a mutex. Exception: %s", strerror(errno));
		exit_s();
	
	}

	free_secure(Params, P_config);
	// Free secure
	signal(SIGINT, sig_secure);
	signal(SIGTERM, sig_secure);
	signal(SIGQUIT, sig_secure);
	signal(SIGUSR1, sig_secure);
	signal(SIGUSR2, sig_secure);

	for (i = 0; i < P_config->users_limit; i++) {
		Params[i].init = 0;
	
	}

	while (1) {
		if (!(n_client < P_config->users_limit)) {
			/* Esperamos a que haya un puesto para nuevos clientes */

			dprintf("Waiting \"%d\" second's for a space in the connections ...",
					P_config->timewait);
			sleep(P_config->timewait);
			continue;

		}

		for (i = 0; i < P_config->users_limit; i++) {
			if (Params[i].init == 0) {
				if ((Params[i].fd = accept(fd, (struct sockaddr *)&Client,
										   &sin_size)) == -1) {
					cprintf(stderr, "Error the request accepting -> %s:%d",
							inet_ntoa(Client.sin_addr), ntohs(Client.sin_port));
					continue;
				
				}

				Params[i].addr = inet_ntoa(Client.sin_addr);
				Params[i].port = ntohs(Client.sin_port);
				Params[i].P_config = P_config;
				Params[i].n_client = &n_client;
				Params[i].init = 1; /* Significa que está en ejecución */

				pthread_create(&Params[i].n_thread, NULL,
							   thread_interact, (void *)&Params[i]);

				pthread_detach(Params[i].n_thread);

				dprintf("FD:%d; Thread:%p; Connected -> %s:%d", 
						Params[i].fd, (void *)Params[i].n_thread, 
						Params[i].addr, Params[i].port);

				n_client += 1;

			}
		
		}

	}

}
