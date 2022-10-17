#include "libft.h"
#include <stdio.h>
#include <asm/file.h>
#include <asm/parser.h>
#include <assert.h>

static int usage(void)
{
	dprintf(2, "Usage: ./asm [file]\n");
	dprintf(2, "Where [file] can be any assembler file which extension is .s.\n");

	return 1;
}

void assemble(struct asm_file *f)
{
	struct parser p;
	p.input = f;
	p.lns = NULL;
	p.nb = 0;

	parse(&p);
	// assemble...
}

int main(int ac, char **av)
{
	if (ac != 2)
		return usage();
	struct asm_file *f = setup_file(av[1]);

	assert(f != NULL); // Should never crash the program
	assemble(f);
	return 0;
}
