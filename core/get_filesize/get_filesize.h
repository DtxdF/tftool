#ifndef _GET_FILESIZE
#define _GET_FILESIZE

// Sé que es una exageración, pero ¿Quien sabe? =D...

#define KiB 1024
#define MiB 1048576
#define GiB 1073741824
#define TiB 1099511627776
#define PiB 1125899906842624
#define EiB 1152921504606846976

struct filesize {

	char * unit;
	long double size;
	long double aux;

};

void get_filesize(struct filesize * fz_p);

#endif
