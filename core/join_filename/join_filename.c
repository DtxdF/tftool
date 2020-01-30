/*	join_filename.c
 *
 *	Crea una ruta absoluta a partir del directiorio de trabajo y el nombre
 *	del archivo a operar.
 *
*/

#include <string.h>

void join_filename(char * path, char * dir_name, char * file_name,
				   size_t path_length) {
	strncpy(path, dir_name, path_length);
	strncat(path, "/", path_length);
	strncat(path, file_name, path_length);

	return;

}
