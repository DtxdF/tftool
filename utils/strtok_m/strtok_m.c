/*	strtok_m.c
 *
 *	Tokenizar un string, al igual que strtok, sólo que es util para
 *	subprocesos y no modifica ninguna cadena.
 *
*/

#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "strtok_m.h"
#include "../../core/strlen_m/strlen_m.h"
#include "../../core/strip/strip.h"

/* Remplaza un delimitador por un espacio */

void erase_com(char * string, char delim) {
	size_t length;
	unsigned long int i;

	length = strlen_m(string);

	for (i = 0; i < length; i++) {
		if (string[i] == delim) {
			string[i] = ' ';
		
		}
	
	}

}

/* Verifica si existe un delimitador en un string */

short int check_char(char * string, char delim) {
	int i;

	i = 0;

	while ((string[i] != '\0')) {
		if (string[i] == delim) {
			return 1;
		
		}

		i += 1;
	
	}

	return 0;

}

char * strtok_m(char * buff, size_t array_length, char * array,
				struct tok * Tok, char delim) {
	size_t buff_length;

	// Si no se usa, causaría una violación de segmento O_O

	if ((array[0] == ' ') || (array[0] == delim)) {
		return NULL;
	
	}

	// Verificamos que esté presente el delimitador, si no lo está, retorna
	// el mismo string.

	if (!check_char(array, delim)) {
		Tok->token = array;

		return NULL;
	
	}

	for (; Tok->i < array_length; Tok->i++, Tok->j++) {
		buff[Tok->j] = array[Tok->i];
		
		// Verificamos que el delimitador esté presento un índice+1 que el
		// actual y que no esté presente dos índices después.

		if ((array[Tok->i+1] == delim) && (array[Tok->i+2] != delim)) { 
			buff[Tok->j+1] = '\0'; /* Se termina de tokenizar */
			buff_length = strlen_m(buff);

			Tok->i += 2;
			Tok->j = 0;

			erase_com(buff, delim); /* Rellenamos con espacios en blanco lo que coincida con el delimitador */
			strip(buff, buff_length); /* Quitamos los espacios en blanco iniciales y finales */

			if (strncmp(buff, "", buff_length) == 0) { /* Verifico que sea una cadena basura */
				continue;
			
			}

			Tok->token = buff;

			return Tok->token;
		
		}


	}

	// El mismo procedimiento con el último token

	erase_com(Tok->token, delim);
	strip(Tok->token, strlen_m(Tok->token));

	return NULL;

}
