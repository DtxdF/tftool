/*	cronometer.c
 *
 *	Captura la hora actual
 *
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * init_cronometer(char * buff, size_t buff_length) {
	struct tm * time_struct;

	time_t time_info;

	time_info = time(NULL);
	time_struct = localtime(&time_info);

	strftime(buff, buff_length, "%H:%M:%S", time_struct);

	return buff;

}
