#define _GNU_SOURCE

#include "asm/parser.h"
#include "asm/ast.h"
#include <libft.h>
#include <stdio.h>
#include "common/op.h"

bool ln_is_useful(char *ln)
{
	if (!ft_strlen(ln))
		return false;
	for (; *ln; ln++)
	{
		if (*ln == '#')
			return false;
		if (*ln != '#' && !(9 <= *ln && *ln <= 13))
			return true;
	}
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
	char **lines = split_lines(p->input->content);

	if (!lines)
	{
		dprintf(2, "Error: split failed.\n");
		exit(1);
	}
	p->nb = count_lines(lines);
	p->lns = ft_calloc(p->nb, sizeof *p->lns);

	char *dot = ft_strrchr(p->input->filename, '.');
	const char *start = ft_strrchr(p->input->filename, '/');
	start = start ? start + 1 : p->input->filename;
	size_t len = dot - start;
	for (size_t i = 0, j = 0; lines[i]; i++)
	{
		if (ln_is_useful(lines[i]))
		{
			p->lns[j].original = lines[i];
			p->lns[j].ln_nb = i + 1;
			if (!tokenize(p->lns + j))
			{
				print_errors(p->lns + j);
				continue;
			}
			char filename[512];
			snprintf(filename, sizeof filename,
				"resources/dot_files/%.*s_%05zu.dot", (int)len, start, p->lns[j].ln_nb);
			printf("\033[1;31mTokens before contextual analysis:\033[0m\n");
			dump_tokens(filename, p->lns[j].original, p->lns[j].tokens);

			bool res = build_ast(p->lns + j);

			printf("\033[1;31mTokens after contextual analysis:\033[0m\n");
			dump_tokens(filename, p->lns[j].original, p->lns[j].tokens);

			dump_ast(filename, p->lns + j, p->input->filename);
			printf("\n");
			printf("Build ast returned \033[1m%s\033[0m\n", res ? "true" : "false");

			j++;
		}
		else
			free(lines[i]);
	}
	free(lines);
}
