#define _GNU_SOURCE

#include "asm/parser.h"
#include <libft.h>
#include <stdio.h>
#include "common/op.h"

bool ln_is_useful(char *ln)
{
	if (!ft_strlen(ln))
		return false;
	for (; *ln; ln++)
		if (*ln != '#' && !(9 <= *ln && *ln <= 13))
			return true;
	return false;
}

static size_t count_lines(char **lines)
{
	size_t ln = 0;
	for (size_t i = 0; lines[i]; i++)
	{
		if (ln_is_useful(lines[i]))
			ln++;
	}
	return ln;
}

void lexer(struct parser *p)
{
	char **lines = ft_split(p->input->content, '\n');

	if (!lines)
	{
		dprintf(2, "Error: split failed.\n");
		exit(1);
	}
	p->nb = count_lines(lines);
	p->lns = ft_calloc(p->nb, sizeof *p->lns);

	for (size_t i = 0, j = 0; lines[i]; i++)
	{
		if (ln_is_useful(lines[i]))
		{
			p->lns[j].tokens = lst_token_new();
			if (!p->lns[j].tokens)
			{
				dprintf(2, "Error: Cannot allocate a new list.\n");
				exit(EXIT_FAILURE);
			}
			p->lns[j].original = lines[i];
			tokenize(p->lns[j].original, p->lns[j].tokens);
			dump_tokens(p->lns[j].original, p->lns[j].tokens);
			j++;
		}
		else
			free(lines[i]);
	}
	free(lines);
}
