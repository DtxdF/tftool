/*	client.c
 *
 *	Es una utilidad que le proporciona al servidor un poder más;
 *	conectarse a más servidores con el fin de guardar el archivo
 *	que es envíado por última vez.
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
#include <errno.h>

#include "../../conf/config.h"

#include "../../core/strlen_m/strlen_m.h"
#include "../../core/cronometer/cronometer.h"
#include "../../core/get_filesize/get_filesize.h"

#include "../conf_parser/conf_parser.h"
#include "../strtok_m/strtok_m.h"
#include "../debug/debug.h"
#include "../strtohost/strtohost.h"
#include "../interact/interact.h"
#include "../skills/check_error/check_error.h"
#include "../exit_secure/exit_secure.h"

int cli_connect(char * addr, unsigned short int port) {
	struct sockaddr_in Client;
	struct hostent * hs;

	int fd;

	if ((hs = gethostbyname(addr)) == NULL) {
		return -2;
	
	}

	if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		return -1;
	
	}

	Client.sin_family = AF_INET;
	inet_aton(inet_ntoa(*((struct in_addr *)hs->h_addr)), &(Client.sin_addr));
	Client.sin_port = htons(port);
	memset(Client.sin_zero, 0, 8);

	return connect(fd, (struct sockaddr *)&Client, sizeof(struct sockaddr)) == -1 ? -1 : fd;

}

void sendall(int fd, char * filename, char * addr, unsigned short int debug) {
	struct filesize fz;

	char buff[MAX_BUFF_RESPONSE];
	char errcod[1];
	char * ptr;

	FILE * file;

	int recv_length;
	int recv_sock;

	fz.size = 0;

	file = fopen(filename, "rb");

	if (file == NULL) {
		cprintf(stderr, "%s: %s", filename, strerror(errno));

		return;
	
	}

	dprintf("Forwarding %s -> %s", filename, addr);

	send(fd, "u:", sizeof(char)*2, 0);
	send(fd, filename, strlen_m(filename), 0);

	if ((recv_sock = recv(fd, errcod, sizeof(errcod), 0)) <= 0) {
		recv_sock == -1 ? cprintf(stderr, "%s: %s",
								  addr, strerror(errno)) : cprintf(stderr, "%s: A possible desconnection has ocurred!", addr);

		return;
	
	}

	cprintf(stderr, "Status: %s",
			check_error(strtol(errcod, &ptr, 10)));

	while (!ferror(file) && !feof(file)) {
		recv_length = fread(buff, sizeof(char),
							sizeof(buff), file);

		fz.size += recv_length;

		if (send(fd, buff, recv_length, 0) == -1) {
			dprintf("%s: %s", addr, strerror(errno));

			break;
		
		}

		get_filesize(&fz);

		if (debug >= 1) {
			dprintf("Forwarded %d -> %.3LF%s to %s",
					recv_length, fz.aux, fz.unit, addr);

		}

	}

	dprintf("Done!. Total: %d Bytes -> %.3LF%s",
			recv_length, fz.aux, fz.unit);

}

void cli_interact(char * addr, char * filename, unsigned short int debug) {
	struct strtohost sth;

	int fd;

	str_to_host(addr, &sth);

	if (!sth.err) {
		if ((fd = cli_connect(sth.addr, sth.port)) == -1) {
			cprintf(stderr, "Error connecting to host \"%s\". Exception: %s",
					addr, strerror(errno));

			return;

		}

		sendall(fd, filename, addr, debug);

	} else {
		cprintf(stderr, "%s: %s\n", addr, sth.errmsg);
	
	}

}

void cli_send(char * filename, struct configuration * Config) {
	struct tok Tok;

	char * aux;
	char * token;
	char * backup;
	char * hosts;

	size_t array_length;

	hosts = Config->backup;

	if (strncmp(hosts, "", strlen_m(hosts)) == 0) {
		return;
	
	}

	Tok.token = NULL;
	Tok.i = 0;
	Tok.j = 0;

	backup = NULL;

	array_length = strlen_m(hosts);
	aux = (char *)malloc(array_length);

	if (aux == NULL) {
		perror("tftool");
		exit_s();

		return;
	
	}

	token = strtok_m(aux, array_length, hosts, &Tok, ',');

	do {
		if ((token == NULL) && (Tok.token == NULL)) {
			break;
		
		}

		cli_interact(Tok.token, filename, Config->debug);
	
		if (token != NULL) {
			backup = token;
		
		}

	} while((token = strtok_m(aux, array_length, hosts, &Tok, ',')));

	if (backup != NULL) {
		cli_interact(backup, filename, Config->debug);

	}

	free(aux);

}
