#define _GNU_SOURCE
#include <stdio.h>

#include "asm/parser.h"

void ln_add_error(struct line *ln, enum ln_error type, char *val)
{
	ln->errors = realloc(ln->errors, (++ln->nb_errors) * sizeof *ln->errors);
	if (!ln->errors)
	{
		dprintf(2, "Error: Couldn't allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	ln->errors[ln->nb_errors - 1].err_chr = 0;
	ln->errors[ln->nb_errors - 1].err = type;
	ln->errors[ln->nb_errors - 1].str = val;
}