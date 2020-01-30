/*	tftool.c
 *
 *	tftool (Transfer tool). Es una herramienta para la transferencia de
 *	archivos tratando de ser lo más minimalista posible
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include "conf/config.h"

#include "utils/conf_parser/conf_parser.h"
#include "utils/interact/interact.h"

int main(void) {
	struct configuration config;

	parser(&config); /* Ajustamos los valores de la configuración */

	struct sockaddr_in Server;
	struct hostent * hst;
	int sock_server;

	// Declaramos los valores del socket

	int reuse_addr_option = 1;
	int max_recv;
	int max_send;

	reuse_addr_option = 1;
	max_recv = MAX_BUFF_RECV;
	max_send = MAX_BUFF_SEND;

	if ((hst = gethostbyname(config.host)) == NULL) {
	    fprintf(stderr, "Error resolving the hostname: \"%s\"\n", config.host);
	    return EXIT_FAILURE;

	}

	if ((sock_server = socket(AF_INET, SOCK_STREAM,
				  IPPROTO_TCP)) == -1) {
	    perror("Socket");
	    return EXIT_FAILURE;
	
	}

	Server.sin_family = AF_INET;
	inet_aton(inet_ntoa(*((struct in_addr *)hst->h_addr)), &(Server.sin_addr));
	Server.sin_port = htons(config.port);
	memset(Server.sin_zero, 0, sizeof(Server.sin_zero));

	// Ajustamos los valores al socket

	setsockopt(sock_server, SOL_SOCKET, SO_REUSEADDR,
		   &reuse_addr_option, sizeof(reuse_addr_option));
	setsockopt(sock_server, SOL_SOCKET, SO_SNDBUF,
		   &max_recv, sizeof(max_recv));
	setsockopt(sock_server, SOL_SOCKET, SO_RCVBUF,
		   &max_send, sizeof(max_send));

	if (bind(sock_server, (struct sockaddr *)&Server,
		 sizeof(struct sockaddr)) == -1) {
	    perror("Bind");
	    return EXIT_FAILURE;

	}

	if (listen(sock_server, 1) == -1) {
	    perror("Listen");
	    return EXIT_FAILURE;
	
	}

	printf("Listening on %s:%d\n",
		config.host, config.port);

	interact(sock_server, &config);

	return EXIT_SUCCESS;

}
