#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "../../conf/config.h"

#include "../../core/inih/ini.h"
#include "../../core/strlen_m/strlen_m.h"

#include "../exit_secure/exit_secure.h"
#include "conf_parser.h"

int handler(void * user, const char * section, const char * name,
    const char * value) {
    struct configuration * ptr_config;

    char * ptr_tmp;

    int section_l;
    int name_l;

    ptr_config = (struct configuration *)user;
    section_l = sizeof(section);
    name_l = sizeof(name);

    if (CFG_MATCH("ADDRESSING", "host")) {
		strncpy(ptr_config->host, value, sizeof(ptr_config->host));
    
    } else if (CFG_MATCH("ADDRESSING", "port")) {
		ptr_config->port = strtol(value, &ptr_tmp, 10);;

    } else if (CFG_MATCH("ROOT", "root_folder")) {
		ptr_config->root_folder = strdup(value);
    
    } else if (CFG_MATCH("ROOT", "debug")) {
    	ptr_config->debug = strtol(value, &ptr_tmp, 10);
    
    } else if (CFG_MATCH("ROOT", "output")) {
		ptr_config->output = strdup(value);
	
	} else if (CFG_MATCH("ROOT", "timeout")) {
    	ptr_config->timeout = strtol(value, &ptr_tmp, 10);
    
    } else if (CFG_MATCH("ACTIONS", "extension_only")) {
		ptr_config->extension_only = strdup(value);

    } else if (CFG_MATCH("ACTIONS", "action")) {
    	strncpy(ptr_config->action, value, sizeof(ptr_config->action));
    
    } else if (CFG_MATCH("ACTIONS", "timewait")) {
		ptr_config->timewait = strtol(value, &ptr_tmp, 10);
    
    } else if (CFG_MATCH("ACTIONS", "users_limit")) {
		ptr_config->users_limit = strtol(value, &ptr_tmp, 10);
    
    } else if (CFG_MATCH("ACTIONS", "backup")) {
    	ptr_config->backup = strdup(value);
    
    } else {
		return 0;
    
    }

    return 1;

}

void parser(struct configuration * P_config) {
    if (ini_parse(CONF_FILE, handler, P_config) < 0) {
		fprintf(stderr, "An error has ocurred reading the file configuration. ErrorCode:%d - ErrorMSG:%s\n",
				errno, strerror(errno));
		exit_s();
    
    }

}
