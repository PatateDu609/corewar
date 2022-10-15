#define _GNU_SOURCE
#include <libft.h>
#include "asm/error.h"
#include <stdio.h>

void print_errors(struct line *ln)
{
	printf("\033[1;31mError:\033[0m \033[33mLine %zu\033[0m:\n", ln->ln_nb);
	for (size_t i = 0; i < ln->nb_errors; i++)
	{
		dprintf(2, "\n - ");
		size_t len = ft_strlen(ln->errors[i].str);
		if (ln->errors[i].err == LN_ERR_STRAY_TOKEN)
		{
			char *str = len == 1 ? "character" : "token";
			dprintf(2, "Stray %s '%s'\n", str, ln->errors[i].str);
		}
	}
}