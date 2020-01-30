#ifndef _STRTOK_M
#define _STRTOK_M
struct tok {

	char * token;
	unsigned long int i;
	unsigned long int j;

};

char * strtok_m(char * buff, size_t array_length,
				char * array, struct tok * Tok, char delim);
#endif
