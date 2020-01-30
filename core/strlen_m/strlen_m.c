/*	strlen_m.c
 *
 *	Cuenta hasta el carácter nulo para saber la longitud de un string y le
 *	suma uno (util para algunos casos en que se desee sacar la longitud
 *	más el carácter nulo).
 *
*/

#include <stddef.h>

size_t strlen_m(const char * string) {
	size_t i;

	for (i = 0; string[i] != '\0'; i++);

	return i+1;

}
