/*	get_filesize.c
 *
 *	Calcula la longitud de bytes y lo transformaa una unidad más legible*/

#include "get_filesize.h"

void get_filesize(struct filesize * fz_p) {
	// fz_p->aux. Es lo que se mostrará, pero 'fz_p->size' es la longitud
	// en bytes.

	if ((fz_p->size < KiB) || fz_p->size > EiB) {
		fz_p->aux = fz_p->size;
		fz_p->unit = "B";

	} else if ((KiB <= fz_p->size) && (fz_p->size < MiB)) {
		fz_p->aux = fz_p->size/KiB;
		fz_p->unit = "KiB";
	
	} else if ((MiB <= fz_p->size) && (fz_p->size < GiB)) {
		fz_p->aux = fz_p->size/MiB;
		fz_p->unit = "MiB";
	
	} else if ((GiB <= fz_p->size) && (fz_p->size < TiB)) {
		fz_p->aux = fz_p->size/GiB;
		fz_p->unit = "GiB";
	
	} else if ((TiB <= fz_p->size) && (fz_p->size < PiB)) {
		fz_p->aux = fz_p->size/TiB;
		fz_p->unit = "TiB";

	} else if ((PiB <= fz_p->size) && (fz_p->size < EiB)) {
		fz_p->aux = fz_p->size/PiB;
		fz_p->unit = "PiB";

	} else if (EiB <= fz_p->size) {
		fz_p->aux = fz_p->size/EiB;
		fz_p->unit = "EiB";
	
	} else {
		fz_p->aux = 0;
		fz_p->unit = "U"; // (U)nknown/Desconocido
	
	}

}
