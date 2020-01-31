/* No recomiendo tocar nada a menos que sepa lo que hace */

#ifndef _GLOBAL_CONF
#define _GLOBAL_CONF

#define CONF_FILE		"/etc/tftool/gconf.cfg" /* El archivo de configuración */

/* Los siguientes valores iran a las opciones del socket */

#define MAX_BUFF_SEND		1024
#define MAX_BUFF_RECV		1024

/* ¡Tenga cuidado con el stack!. Por favor ¡no coloque valores tan grandes! */

#define MAX_BUFF_RESPONSE	1024
#define MAX_BUFF_REQUEST	1024

/* No coloque tantos para que haya suficientes descriptores de archivos para los archivos en operación */

#define MAX_USER_LIMITS		512

#endif
