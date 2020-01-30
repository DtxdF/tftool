#ifndef _CUSTOM_DEBUG
#define _CUSTOM_DEBUG

/* El buffer que usará "init_cronometer" */

char buff_time[12];

/* 
 * cprintf: Macro principal para mostrar la información con el tiempo. Necesita
 * 			que le indiquen en que archivo será enviano el contenido.
 *
 * dprintf: Toda la salida será enviada a "stdout", pero tendrá el mismo
 * 			comportamiento que "cprintf".
 *
 * vprintf: Agregará la dirección y el puerto mas el descriptor de archivo.
 *
 * eprintf: Igual que vprintf, pero lo enviará a "sderr".
 *
*/

#define		cprintf(std, format, ...) 	fprintf(std, "[%s]:" format " " "\n", \
										  		init_cronometer(buff_time, sizeof(buff_time)), ##__VA_ARGS__)
#define		dprintf(format, ...)		cprintf(stdout, format, ##__VA_ARGS__)

#define		vprintf(format, ...) 		dprintf("(%s:%d) (FD:%d): " format, \
												CParams->addr, CParams->port, CParams->fd, ##__VA_ARGS__)

#define		eprintf(format, ...)		cprintf(stderr, "(%s:%d) (FD:%d): " format, \
									 			CParams->addr, CParams->port, CParams->fd, ##__VA_ARGS__)
#endif
