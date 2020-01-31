#ifndef _CONF_PARSER
#define _CONF_PARSER

#include <pthread.h>

struct configuration {

	// Server configuration
	char host[64];
	unsigned short int port;
	// Folder configuration
	char * root_folder;
	unsigned short int debug;
	char * output;
	unsigned int timeout;
	// Actions
	char action[32];
	char * extension_only;
	unsigned int timewait;
	unsigned int users_limit;
	char * backup;
	// MISC
	pthread_mutex_t * mutex;

};

void parser(struct configuration * P_config);
#endif
