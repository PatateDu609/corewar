#include "libft.h"
#include <stdio.h>
#include "asm/file.h"
#include "asm/parser.h"
#include "asm/encoder.h"
#include <assert.h>
#include "asm/ast.h"

static int usage(void)
{
	dprintf(2, "Usage: ./asm [file]\n");
	dprintf(2, "Where [file] can be any assembler file which extension is .s.\n");

	return 1;
}

static void free_asm(struct parser *p)
{
	free_file(p->input);
	for (size_t i = 0; i < p->nb; i++)
	{
		free_tokens(p->lns[i].tokens);
		free_ast(p->lns[i].ast);
		free(p->lns[i].original);
		free(p->lns[i].errors);
	}
	free(p->lns);
}

static bool load(char *filename, struct parser *p)
{
	struct asm_file *f = setup_file(filename);
	if (!f)
		return false; // Should never happen

	p->input = f;
	p->lns = NULL;
	p->nb = 0;
	return true;
}

int main(int ac, char **av)
{
	if (ac != 2)
		return usage();
	struct parser p;

	int exit_code = load(av[1], &p) && parse(&p) && encode(&p)
		? EXIT_SUCCESS : EXIT_FAILURE;
	free_asm(&p);
	return exit_code;
}
