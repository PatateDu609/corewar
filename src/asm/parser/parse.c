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

static char **setup_lines(struct parser *p)
{
	char **lines = split_lines(p->input->content);

	if (!lines)
	{
		dprintf(2, "Error: split failed.\n");
		exit(EXIT_FAILURE);
	}
	p->nb = count_lines(lines);
	if (!p->nb)
	{
		free(lines);
		return NULL;
	}
	p->lns = ft_calloc(p->nb, sizeof *p->lns);
	if (!p->lns)
	{
		dprintf(2, "Error: Cannot allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	return lines;
}

#define parse_step(step, ln, res)	\
	if (!step(ln))					\
	{								\
		print_errors(ln);			\
		res = false;				\
		continue;					\
	}

static void fill_filename_dotfile(struct parser *p, char *filename, size_t mlen, size_t ln_nb)
{
	static char *dot = NULL;
	static const char *start = NULL;
	static size_t len = 0;
	static char *path = "resources/dot_files";

	if (!dot)
	{
		dot = ft_strrchr(p->input->filename, '.');
		start = ft_strrchr(p->input->filename, '/');
		start = start ? start + 1 : p->input->filename;
		len = dot - start;
	}

	snprintf(filename, mlen, "%s/%.*s_%05zu.dot", path, (int)len, start, ln_nb);
}

bool parse(struct parser *p)
{
	char **lines = setup_lines(p);
	if (!lines)
		return false;

	bool res = true;
	for (size_t i = 0, j = 0; lines[i]; i++, j++)
	{
		if (ln_is_useful(lines[i]))
		{
			p->lns[j].original = lines[i];
			p->lns[j].ln_nb = i + 1;

			char filename[512];
			fill_filename_dotfile(p, filename, sizeof filename, p->lns[j].ln_nb);

			parse_step(tokenize, p->lns + j, res)
			parse_step(build_ast, p->lns + j, res)

			dump_ast(filename, p->lns + j, p->input->filename);
			// dump_tokens(filename, p->lns[j].original, p->lns[j].tokens);

			parse_step(is_valid, p->lns + j, res)
		}
		else
		{
			j--;
			free(lines[i]);
		}
	}
	free(lines);
	return res;
}

#undef parse_step