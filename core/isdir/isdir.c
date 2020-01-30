#include <dirent.h>
#include <stddef.h>
#include <errno.h>

unsigned short int isdir(char * dir_name) {
	DIR * directory;

	directory = opendir(dir_name);

	if (directory == NULL) {
		return errno == ENOTDIR ? 0 : -1;
	
	}

	closedir(directory);

	return 1;

}

