#ifndef _PARSER_CONTENT
#define _PARSER_CONTENT

#include "../../conf/config.h"

struct parser {

	char cmd[3];
	char filename[252];
	char content[MAX_BUFF_REQUEST];
	char response[MAX_BUFF_RESPONSE];
	char * path;

};

int parse_content(struct parser * Parsed, struct client_params * CParams);

#define MATCH(c) strncmp(Parsed.cmd, c, sizeof(Parsed.cmd)) == 0
#define MATCH_P(c) strncmp(Parsed->cmd, c, sizeof(Parsed->cmd)) == 0
#endif
