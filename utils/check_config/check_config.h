#ifndef 	_CHECK_CONFIG
#define 	_CHECK_CONFIG

#include <stdio.h>

#include "../conf_parser/conf_parser.h"

void check_config(struct configuration * Config);

#ifdef		_CHECK_CONFIG_DEBUG

#include <stdio.h>

#define		dprintf(text)		fprintf(stderr, "%s\n", text)

#endif

#ifdef		_CHECK_CONFIG_FUNCS

#include <string.h>

#define		isincorrect(n)	n <= 0
#define		isempty(s)		strlen(s) == 0

#endif

#endif
