/*	get_extension.c
 *
 *	Obtener la extensión de un nombre de archivo.
 *
 */

#include <string.h>

char * get_extension(char * filename) {
	char * ext;

	ext = strrchr(filename, '.');

	if (ext) {
		return ext;
	
	} else {
		return NULL;
	
	}

}
