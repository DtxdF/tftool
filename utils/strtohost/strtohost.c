/*	strtohost.c
 *
 *	Separa un token que siga la siguiente sintaxis "<host/IP>:<port>" en
 *	una estructura para una mejor manipulación.
 *
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "strtohost.h"
#include "../../core/strlen_m/strlen_m.h"

/* Cuenta cuantos puntos dobles tiene el string */

int count_double_points(char * str) {
	register int i;
	register int count;

	for (i = 0, count = 0; str[i] != '\0'; i++) {
		if (str[i] == ':') {
			count += 1;
		
		}
	
	}

	return count;

}

/* Detecta si hay un carácter nulo después de los puntos doble */

short int detect_null_char(char * str) {
	register int i;

	int result;

	for (i = 0, result = 0; str[i] != '\0'; i++) {
		if (str[i] == ';') {
			if (str[i+1] == '\0') {
				result = 1;
				break;
			
			} else {
				break;
			
			}
		
		}
	
	}

	return result;

}

/* Retorna la cadena especificamente con el host/IP */

char * return_addr(char * str, char * buff, size_t buff_length) {
	register int i;

	for (i = 0; str[i] != ';' && (i < buff_length); i++) {
		buff[i] = str[i];
	
	}

	buff[i] = '\0';

	return buff;

}

/* Retorna el número de puerto */

unsigned short int return_port(char * str, int init) {
	char * ptr;
	char buff[6];

	register int i;
	register int j;

	long int port;

	for (i = init, j = 0; j < sizeof(buff); i++, j++) {
		buff[j] = str[i];
	
	}

	port = strtol(buff, &ptr, 10);

	return (unsigned short int)port;

}

/* Retorna la primera vez que hay una coincidencia con los puntos dobles */

int detect_first_double_points(char * str) {
	register int i;

	for (i = 0; str[i] != '\0'; i++) {
		if (str[i] == ':') {
			break;
		
		}
	
	}

	return i;

}

void str_to_host(char * addr, struct strtohost * sth) {
	int double_points;

	sth->err = 0;

	double_points = count_double_points(addr);

	// Si no cumple con el minimo o maximo de longitud

	if (((strlen_m(addr)-1) < 4) || ((strlen_m(addr)-1) > 68)) {
		sth->err = 1;
		sth->errmsg = "The length of the IP address/Hostname must be a minimum of 4 and a maximum of 68";

		return;
	
	}

	// Si no hay o hay más doble puntos que uno

	if ((double_points == 0) || (double_points > 1)) {
		sth->err = 1;
		sth->errmsg = "The length of the separators must be 1";

		return;
	
	}

	// Detecta si se encuentra el carácter nulo al inicio o los puntos dobles son el carácter final antes del nulo

	if ((detect_null_char(addr)) || (addr[0] == '\0')) {
		sth->err = 1;
		sth->errmsg = "The separators must not end before the null character or the null character must not be in the first index";

		return;
	
	}

	strncpy(sth->addr, return_addr(addr, sth->addr,
								   detect_first_double_points(addr)),
			sizeof(sth->addr));
	sth->port = return_port(addr, strlen_m(sth->addr));

	// Si hay un error con el puerto

	if (sth->port == 0) {
		sth->err = 1;
		sth->errmsg = "There was an error with the port";
	
	}

}
