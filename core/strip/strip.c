/*	strip.c
 *
 *	Eliminar espacios iniciales y finales
 *
 */

#include <string.h>
#include <ctype.h>

void rstrip(char * string, size_t length) {
	if (isspace(string[length-1]) != 0) {
		string[length-1] = '\0';
		rstrip(string, length-1);
	
	}

}

void lstrip(char * string, size_t length) {
	register int i;
	char * aux = string;

	i = 0;

	while (1) {
		if (isspace(string[i]) != 0) {
			aux = aux+(i+1);
		
		} else {
			break;
		
		}

		i += 1;

	}
	
	strncpy(string, aux, length);

}

void strip(char * string, size_t length) {
	rstrip(string, length-1);
	lstrip(string, length);

}
