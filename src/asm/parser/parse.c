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

void parse(struct parser *p)
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
			p->lns[j].original = lines[i];
			p->lns[j].ln_nb = i;
			if (!tokenize(p->lns + j))
				continue ;
			// build_ast(p->lns + j);
			dump_tokens(p->lns[j].original, p->lns[j].tokens);
			j++;
		}
		else
			free(lines[i]);
	}
	free(lines);
}
