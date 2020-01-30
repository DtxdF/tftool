#ifndef _STR_TO_HOST
#define _STR_TO_HOST
struct strtohost {

	char addr[64]; // Esta longitud es la ideal
	unsigned short int port;
	unsigned short int err;
	char * errmsg;

};
void str_to_host(char * addr, struct strtohost * sth);
#endif
