#ifndef		_INTERACT
#define		_INTERACT

#include <pthread.h>

#include "../conf_parser/conf_parser.h"

struct client_params {

	int fd;
	char * addr;
	unsigned short int port;
	struct configuration * P_config;
	int * n_client;
	pthread_t n_thread;
	unsigned short int init;

};

void interact(int fd, struct configuration * P_config);

/* Una pequeña utilidad para mandar un byte e informar al cliente qué está pasando o básicamente un código de estado */

#define		send_notify(response) 	send(CParams->fd, response, sizeof(char), 0)
#define CMP_ACTION(c) check_action(c, CParams->P_config->action, sizeof(CParams->P_config->action))

#endif
