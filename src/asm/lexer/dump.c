#include <asm/parser.h>
#include <stdio.h>

#define BOLD "\e[1m"
#define RED "\e[38;5;124m"
#define BLUE "\e[38;5;20m"
#define GREEN "\e[38;5;46m"
#define RESET "\e[0m"

void dump_tokens(char *line, lst_token_t *toks)
{
	printf("-----------------------------------------------------------------\n");
	printf("\n");

	if (line)
		printf(RED "Original line:" RESET " %s\n", line);

	static char *types[] = {
		"Unknown",
		"Default",
		"Parameter separator",
		"Direct value char",
		"Label char",
		"Header",
		"Register",
		"Number",
		"Word",
		"Instruction",
	};

	for (lst_node_token_t *node = toks->start; node; node = node->next)
	{
		token_t data = node->data;
		printf(BOLD BLUE "node: " RESET BOLD "%s\n" RESET, types[(int)data.type + 1]);
		printf(BOLD BLUE "value: " RESET "%s\n", data.value);

		if (node->next)
			printf("\n");
	}
}
